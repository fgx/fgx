/*\
 * fgx_unz.cpp
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <sys/types.h>
#include <sys/stat.h>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdio.h>
#include <string.h> // for strdup(), ...
#include <errno.h>
#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#include <direct.h> // for _chdir, ...
#else
#include <utime.h> // for utime, ...
#endif
#include "app_config.h"
#include "fgx_gzlib.h"
#include "loadAptDat.h"
#include "unzip.h"
#if defined(WIN32) || defined(_WIN32)
#define USEWIN32IOAPI
#include "iowin32.h"
#endif
#include "fgx_unz.h"

static const char *module = "fgx_unz";

#define CASESENSITIVITY     0
#define WRITEBUFFERSIZE     8192
#define MAXFILENAME         256
#define ERRBUFSIZE          2014


#ifdef __APPLE__
// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#define FTELLO_FUNC(stream) ftello(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif

#ifndef EndBuf
#define EndBuf(a) ( a + strlen(a) )
#endif

static QStringList err_list;
static char err_buff[ERRBUFSIZE];

std::string getUzErrorString()
{
    return err_list.join("").toStdString();
}

static std::string Display64BitsSize(ZPOS64_T n, int size_char)
{
    /* to avoid compatibility problem , we do here the conversion */
    char number[21];
    int offset = 19;
    int pos_string = 19;
    std::ostringstream ss;

    number[20] = 0;

    for (;;) {
        number[offset] = (char)((n%10)+'0');
        if (number[offset] != '0')
            pos_string = offset;
        n /= 10;
        if (offset == 0)
            break;
        offset--;
    }
    {
        int size_display_string = 19 - pos_string;
        while (size_char > size_display_string) {
            size_char--;
            ss << " ";
            //printf(" ");
        }
    }

    sprintf(err_buff,"%s", &number[pos_string]);
    ss << err_buff;
    return ss.str();
}

static int do_list(unzFile uf)
{
    uLong i;
    unz_global_info64 gi;
    int err;

    err = unzGetGlobalInfo64(uf,&gi);
    if (err != UNZ_OK) {
        sprintf(err_buff,"error %d with zipfile in unzGetGlobalInfo \n",err);
        err_list << err_buff;
        return unze_no_get_info;
    }
    sprintf(err_buff,"  Length  Method     Size Ratio   Date    Time   CRC-32     Name\n");
    err_list << err_buff;
    // printf("  ------  ------     ---- -----   ----    ----   ------     ----\n");
    for (i = 0; i < gi.number_entry; i++) {
        char filename_inzip[256];
        unz_file_info64 file_info;
        uLong ratio = 0;
        const char *string_method;
        char charCrypt = ' ';
        std::string s;
        err = unzGetCurrentFileInfo64(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
        if (err != UNZ_OK) {
            sprintf(err_buff,"error %d with zipfile in unzGetCurrentFileInfo\n",err);
            err_list << err_buff;
            return unze_no_file_info;
        }
        if (file_info.uncompressed_size > 0)
            ratio = (uLong)((file_info.compressed_size*100) / file_info.uncompressed_size);

        /* display a '*' if the file is crypted */
        if ((file_info.flag & 1) != 0)
            charCrypt='*';

        if (file_info.compression_method==0)
            string_method="Stored";
        else
        if (file_info.compression_method==Z_DEFLATED)
        {
            uInt iLevel=(uInt)((file_info.flag & 0x6)/2);
            if (iLevel==0)
              string_method="Defl:N";
            else if (iLevel==1)
              string_method="Defl:X";
            else if ((iLevel==2) || (iLevel==3))
              string_method="Defl:F"; /* 2:fast , 3 : extra fast*/
        }
        else
        if (file_info.compression_method==Z_BZIP2ED)
        {
              string_method="BZip2 ";
        }
        else
            string_method="Unkn. ";

        s = Display64BitsSize(file_info.uncompressed_size,7);
        sprintf(err_buff,"  %6s%c",string_method,charCrypt);
        s += err_buff;
        s += Display64BitsSize(file_info.compressed_size,7);
        sprintf(err_buff," %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lx   %s\n",
                ratio,
                (uLong)file_info.tmu_date.tm_mon + 1,
                (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour,(uLong)file_info.tmu_date.tm_min,
                (uLong)file_info.crc,filename_inzip);
        s += err_buff;
        err_list << s.c_str();
        if ((i+1) < gi.number_entry) {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK) {
                sprintf(err_buff,"error %d with zipfile in unzGoToNextFile\n",err);
                err_list << err_buff;
                return  unze_no_get_next;
            }
        }
    }
    return unze_none;
}


/* change_file_date : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
void change_file_date(const char *filename, uLong dosdate, tm_unz tmu_date )
{
#ifdef _WIN32
    HANDLE hFile;
    FILETIME ftm,ftLocal,ftCreate,ftLastAcc,ftLastWrite;

    hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE,
                      0,NULL, OPEN_EXISTING ,0,NULL);
    GetFileTime(hFile,&ftCreate,&ftLastAcc,&ftLastWrite);
    DosDateTimeToFileTime((WORD)(dosdate>>16),(WORD)dosdate,&ftLocal);
    LocalFileTimeToFileTime(&ftLocal,&ftm);
    SetFileTime(hFile,&ftm,&ftLastAcc,&ftm);
    CloseHandle(hFile);
#else
#if defined(unix) || defined(__APPLE__)
    struct utimbuf ut;
    struct tm newdate;
    newdate.tm_sec = tmu_date.tm_sec;
    newdate.tm_min=tmu_date.tm_min;
    newdate.tm_hour=tmu_date.tm_hour;
    newdate.tm_mday=tmu_date.tm_mday;
    newdate.tm_mon=tmu_date.tm_mon;
    if (tmu_date.tm_year > 1900)
       newdate.tm_year = tmu_date.tm_year - 1900;
    else
       newdate.tm_year = tmu_date.tm_year ;
    newdate.tm_isdst = -1;

    ut.actime=ut.modtime = mktime(&newdate);
    utime(filename,&ut);
#else
#error Need port of set file time for this OS
#endif
#endif
}

#ifdef _MSC_VER
#define M_IS_DIR _S_IFDIR
#else // !_MSC_VER
#define M_IS_DIR S_IFDIR
#endif

enum DiskType {
    DT_NONE,
    DT_FILE,
    DT_DIR
};

static struct stat buf;
DiskType is_file_or_directory32 ( const char * path )
{
    if (!path)
        return DT_NONE;
	if (stat(path,&buf) == 0)
	{
		if (buf.st_mode & M_IS_DIR)
			return DT_DIR;
		else
			return DT_FILE;
	}
	return DT_NONE;
}

size_t get_last_file_size32() { return buf.st_size; }
#ifdef _MSC_VER
static struct _stat64 buf64;
DiskType is_file_or_directory64 ( const char * path )
{
    if (!path)
        return DT_NONE;
	if (_stat64(path,&buf64) == 0)
	{
		if (buf64.st_mode & M_IS_DIR)
			return DT_DIR;
		else
			return DT_FILE;
	}
	return DT_NONE;
}

__int64 get_last_file_size64() { return buf64.st_size; }

#define is_file_or_directory is_file_or_directory64
#define get_last_file_size get_last_file_size64
#else
#define is_file_or_directory is_file_or_directory32
#define get_last_file_size get_last_file_size32
#endif

/*\
 *  mymkdir and change_file_date are not 100 % portable
 *  As I don't know well Unix, I wait feedback for the unix portion 
 *  In Windows, _mkdir can create only one new directory per call, so only the last component 
 *  of dirname can name a new directory
\*/

#ifdef WIN32
#define MKDIR _mkdir
#else
#define MKDIR(a) mkdir(a,0775)
#endif

static int mymkdir(const char* dirname)
{
    int ret = unze_none;
    char buff[264];
    DiskType dt;
    int c, i, max = (int)strlen(dirname);
    // in ALL CASES MUST do directory by directory
    for (i = 0; i < max; i++) {
        c = dirname[i];
        if (( c == '\\' ) || ( c == '/' )) {
            buff[i] = 0;
            if (i) {
                dt = is_file_or_directory(buff);
                if (dt != DT_DIR) {
                    if (dt == DT_FILE) {
                        sprintf(err_buff,"Error: Can NOT create directory due to an existing file!\n");
                        err_list << err_buff;
                        return unze_file_in_way;
                    }
                    ret = MKDIR(buff);
                    if (ret) {
                        sprintf(err_buff,"Error creating dir '%s'! (%d)\n", buff, errno);
                        err_list << err_buff;
                        return unze_no_create_dir;
                    }
                }
            }
        }
        buff[i] = (char)c;
    }
    if (i) {
        buff[i] = 0;
        dt = is_file_or_directory(buff);
        if (dt != DT_DIR) {
            if (dt == DT_FILE) {
                sprintf(err_buff,"Error: Can NOT create directory due to an existing file!\n");
                err_list << err_buff;
                return unze_file_in_way;
            }
            ret = MKDIR(buff);
            if (ret) {
                sprintf(err_buff,"Error creating dir '%s'! (%d)\n", buff, errno);
                err_list << err_buff;
                return unze_no_create_dir;
            }
        }
    }
    return ret;
}

static int makedir (char *newdir)
{
    int iret = unze_none;
    char *buffer ;
    char *p;
    int  len = (int)strlen(newdir);

    if (len <= 0)
        return iret;

    buffer = (char*)malloc(len + 2);
    if (buffer == NULL) {
        sprintf(err_buff,"Error allocating %d bytes of memory\n", (len + 2));
        err_list << err_buff;
        return unze_no_memory;
    }

    strcpy(buffer,newdir);

    if (buffer[len-1] == '/') {
        buffer[len-1] = '\0';
    }
    iret = mymkdir(buffer);
    if (iret) {
        sprintf(err_buff,"mymkdir failed to create dir '%s'\n", buffer);
        err_list << err_buff;
        free(buffer);
        return iret;
    }

    p = buffer+1;
    while (1) {
        char hold;

        while(*p && *p != '\\' && *p != '/')
            p++;

        hold = *p;
        *p = 0;
        iret = mymkdir(buffer);
        if (iret) {
            sprintf(err_buff,"mymkdir couldn't create directory '%s'\n",buffer);
            err_list << err_buff;
            free(buffer);
            return iret;
        }
        if (hold == 0)
            break;
        *p++ = hold;
    }
    free(buffer);
    return iret;
}


int do_extract_currentfile(unzFile uf, const int *popt_extract_without_path, 
    int *popt_overwrite, const char *password )
{
    char filename_inzip[256];
    char* filename_withoutpath;
    char* p;
    int err = UNZ_OK;
    FILE *fout = NULL;
    void* buf;
    uInt size_buf;

    unz_file_info64 file_info;
    uLong ratio=0;
    err = unzGetCurrentFileInfo64(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

    if (err != UNZ_OK) {
        sprintf(err_buff,"error %d with zipfile in unzGetCurrentFileInfo\n",err);
        err_list << err_buff;
        return unze_no_file_info;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf == NULL) {
        sprintf(err_buff,"Error allocating %u bytes of memory\n", size_buf);
        err_list << err_buff;
        return unze_no_memory;
    }

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }

    if ((*filename_withoutpath)=='\0')
    {
        if ((*popt_extract_without_path) == 0)
        {
            //printf("creating directory: %s\n", filename_inzip);
            if (mymkdir(filename_inzip) != unze_none) {
                sprintf(err_buff,"mymkdir unable to create '%s'\n", filename_inzip);
                err_list << err_buff;
            }
        }
    }
    else
    {
        const char* write_filename;
        int skip = 0;

        if ((*popt_extract_without_path)==0)
            write_filename = filename_inzip;
        else
            write_filename = filename_withoutpath;

        err = unzOpenCurrentFilePassword(uf,password);
        if (err != UNZ_OK) {
            sprintf(err_buff,"error %d with zipfile in unzOpenCurrentFilePassword\n",err);
            err_list << err_buff;
            free(buf);
            return unze_no_open_curpwd;
        }
#if 0   // 0000000000000000000000000000000000000000000000000000000000
        // TODO: How to support this option
        if (((*popt_overwrite) == 0) && (err == UNZ_OK))
        {
            char rep=0;
            FILE* ftestexist;
            ftestexist = FOPEN_FUNC(write_filename,"rb");
            if (ftestexist != NULL)
            {
                fclose(ftestexist);
                do
                {
                    char answer[128];
                    int ret;

                    printf("The file %s exists. Overwrite ? [y]es, [n]o, [A]ll: ",write_filename);
                    ret = scanf("%1s",answer);
                    if (ret != 1)
                    {
                       exit(EXIT_FAILURE);
                    }
                    rep = answer[0] ;
                    if ((rep >= 'a') && (rep <= 'z'))
                        rep -= 0x20;
                }
                while ((rep != 'Y') && (rep != 'N') && (rep != 'A'));
            }

            if (rep == 'N')
                skip = 1;

            if (rep == 'A')
                *popt_overwrite = 1;
        }
#endif // 0000000000000000000000000000000000000000000000

        if ((skip == 0) && (err == UNZ_OK))
        {
            fout = FOPEN_FUNC(write_filename,"wb");
            /* some zipfile don't contain directory alone before file */
            if ((fout==NULL) && ( (*popt_extract_without_path)==0) &&
                                  (filename_withoutpath != (char*)filename_inzip))
            {
                char c = *(filename_withoutpath-1);
                *(filename_withoutpath-1)='\0';
                makedir((char *)write_filename);
                *(filename_withoutpath-1) = c;
                fout = FOPEN_FUNC(write_filename,"wb");
            }

            if (fout == NULL)
            {
                sprintf(err_buff,"error failed create %s\n",write_filename);
                err_list << err_buff;
                // abort or continue
                free(buf);
                return unze_no_create_file;
            }
        }

        if (fout != NULL)
        {
            //printf(" extracting: %s\n",write_filename);

            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err < 0)
                {
                    sprintf(err_buff,"error %d with zipfile in unzReadCurrentFile\n",err);
                    err_list << err_buff;
                    free(buf);
                    return unze_failed_read;
                }
                if (err > 0) {
                    if (fwrite(buf,err,1,fout) != 1) {
                        sprintf(err_buff,"error in writing extracted file\n");
                        err_list << err_buff;
                        err = UNZ_ERRNO;
                        free(buf);
                        return unze_failed_write;
                    }
                }
            } while (err > 0);
            if (fout)
                    fclose(fout);

            if (err == 0) {
                change_file_date(write_filename,file_info.dosDate,
                                 file_info.tmu_date);
            }
        }

        //if (err == UNZ_OK)
        {
            err = unzCloseCurrentFile (uf);
            if (err != UNZ_OK)
            {
                sprintf(err_buff,"error %d with zipfile in unzCloseCurrentFile\n",err);
                err_list << err_buff;
                free(buf);
                return unze_close_curr;
            }
        }
        //else
        //    unzCloseCurrentFile(uf); /* don't lose the error */
    }

    free(buf);
    return unze_none;
}

static int do_extract( unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* password )
{
    int iret = unze_none;
    uLong i;
    unz_global_info64 gi;
    int err;
    FILE* fout=NULL;

    err = unzGetGlobalInfo64(uf,&gi);
    if (err != UNZ_OK) {
        sprintf(err_buff,"error %d with zipfile in unzGetGlobalInfo \n",err);
        err_list << err_buff;
        return unze_no_get_info;
    }
    for (i = 0; i < gi.number_entry; i++ ) {
        iret = do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password);
        if (iret)
            break;

        if ( (i+1) < gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK)
            {
                printf("error %d with zipfile in unzGoToNextFile\n",err);
                sprintf(err_buff,"error %d with zipfile in unzGoToNextFile\n",err);
                err_list << err_buff;
                return  unze_no_get_next;
            }
        }
    }

    return iret;
}


int fileUnzip( QString &file, QString &dir, uint flag, const char *password )
{
    int iret = 0;
    unzFile uf = 0;
    std::string sdir = dir.toUtf8().constData();
    std::string sfile = file.toUtf8().constData();
    const char *out_dir = sdir.c_str();   // "C:\\GTools\\ConApps\\tests\\build-test-zlib\\temp_uz";
    const char *zip_file = sfile.c_str();   // "C:\\Users\\user\\Downloads\\21.zip";
    int opt_extract_without_path = (flag & uzflg_nopath) ? 1 : 0;
    int opt_overwrite = (flag & uzflg_nopath) ? 0 : 1;

    err_buff[0] = 0;
    err_list.clear();

#ifdef USEWIN32IOAPI
    zlib_filefunc64_def ffunc;
#endif
#ifdef USEWIN32IOAPI
    fill_win32_filefunc64A(&ffunc);
    uf = unzOpen2_64(zip_file,&ffunc);
#else
    uf = unzOpen64(zip_file);
#endif
    if (!uf) {
        sprintf(err_buff,"%s: Cannot open %s\nFile does not exist, or NOT a zip file!\n",module, zip_file);
        err_list << err_buff;
        return unze_no_zip_file;
    }

    // printf("%s: %s opened, and found central directory.\n", module, zip_file);
    if (flag & uzflg_listonly) {
        iret = do_list(uf);
        unzClose(uf);
        return iret;
    }
    DiskType dt = is_file_or_directory(out_dir);
    if (dt != DT_DIR) {
        sprintf(err_buff,"Directory '%s' does NOT exist!\n", out_dir);
        err_list << err_buff;
        // could return here, but give it a go
    }
#ifdef _WIN32
    if (_chdir(out_dir))
#else
    if (chdir(out_dir))
#endif
    {
        const char *err_type = "Unknown";
        switch (errno) {
        case ENOENT:
            err_type = "Unable to locate";
            break;
        case EINVAL:
            err_type = "Invalid buffer";
            break;
        }
        sprintf(err_buff,"%s: Error %s, changing into '%s', aborting\n", module, err_type, out_dir);
        err_list << err_buff;
        return unze_no_chdir;
    }
    //printf("%s: changed to work directory '%s', and beginning extraction...\n", module, out_dir);
    iret = do_extract( uf, opt_extract_without_path, opt_overwrite, password );

    return iret;
}

// eof = fgx_unz.cpp
