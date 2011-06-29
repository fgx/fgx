

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QVariant>

#include <QtGui/QDesktopServices>
#include <QtGui/QWidget>

#include "xsettings.h"
#include "utilities/utilities.h"
#include "utilities/osdab/zip_p.h"
#include "utilities/osdab/zip.h"
#include "utilities/osdab/unzip.h"
#include "/usr/include/zlib.h"

#define UNZIP_READ_BUFFER (256*1024)


XSettings::XSettings(QObject *parent) :
    QSettings(parent)
{
}


//===========================================================================
//** fgx Install path
//===========================================================================
QString XSettings::fgx_path(){
	return QDir::currentPath().append("/fgx.app/Contents/MacOS");
}


//===========================================================================
//** fgfs Executable Full Path
//===========================================================================
QString XSettings::fgfs_path(){
	return this->value("FGFS", default_fgfs_path()).toString();
}

QString XSettings::default_fgfs_path(){

	if(runningOS() == MAC){
		return QDir::currentPath().append("/fgx.app/Contents/MacOS/fgfs");

	}else if(runningOS() == LINUX){
                return QString("fgfs");
        }
	else if(runningOS() == WINDOWS){
                return QString("C:/Program Files/FlightGear/bin/win32/fgfs.exe");
        }

	return QString("/usr/bin/local/fgfs");
}

//===========================================================================
//** fg_root()
//===========================================================================
QString XSettings::fg_root(){
	return this->value("FG_ROOT", default_fg_root()).toString();
}

QString XSettings::fg_root(QString append_path){
    return this->fg_root().append(append_path);
}

QString XSettings::default_fg_root(){
	if(runningOS() == MAC){
		return QDir::currentPath().append("/fgx.app/Contents/Resources/fgx-fgdata");

	}else if(runningOS() == LINUX){
		return QString("/usr/share/games/FlightGear");
        }
	
	else if(runningOS() == WINDOWS){
                return QString("C:/Program Files/FlightGear/data");
        }

	return QString("TODO - default_fg_root()");
}

//===========================================================================
//** Paths Sane
//===========================================================================
bool XSettings::paths_sane(){
	if(value("USE_DEFAULT_PATHS").toBool()){
		return true;
	}
	if(!QFile::exists(fgfs_path())){
		return false;
	}
	if(!QFile::exists(fg_root())){
		return false;
	}
	return true;
}


//===========================================================================
//** Aircraft Path
//===========================================================================
QString XSettings::aircraft_path(){
	return fg_root().append("/Aircraft");
	outLog("*** FGx settings: Aircraft path: " + fg_root().append("/Aircraft") + " ***");
}

QString XSettings::aircraft_path(QString dir){
	return fg_root().append("/Aircraft/").append(dir);
	outLog("*** FGx settings: Aircraft path: " + fg_root().append("/Aircraft/").append(dir) + " ***");
}


//===========================================================================
//** Airports path
//===========================================================================
QString XSettings::airports_path(){

	//* Using terrasync
	if(use_terrasync()){
		if(runningOS() == MAC){
			QString terrascenehome(QDir::homePath());
			terrascenehome.append("/Documents/TerrasyncScenery");
			return terrascenehome;
			outLog("*** FGx settings: Airports path: " + terrascenehome + " ***");
		}else{
			//* Use the terra sync path
			return terrasync_sync_path().append("/Airports");
			outLog("*** FGx settings: Airports path: " + terrasync_sync_path().append("/Airports") + " ***");
		}
	}
	//* Otherwise return the FG_ROOT airports/
	return fg_root().append("/Scenery/Airports");
	outLog("*** FGx settings: Airports path: " + fg_root().append("/Scenery/Airports") + " ***");
}

//** Apt Dat
QString XSettings::apt_dat_file(){
	QString aptdatloc(QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath().append("/apt.dat"));
	if( QFile::exists(aptdatloc) == false){
		uncompress(fg_root("/Airports/apt.dat.gz"), QString(QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath().append("/apt.dat")));
		outLog(QString(temp_dir().append("apt.dat")));
	}
	return aptdatloc;
}

//===========================================================================
//** Scenery Path - TODO x/win
//===========================================================================
QString XSettings::scenery_path(){

	return fg_root("/Scenery");
}

//===========================================================================
//** TerraSync
//===========================================================================
bool XSettings::use_terrasync(){
	return value("use_terrasync").toBool();
}
QString XSettings::terrasync_exe_path(){
	if (runningOS() == MAC) {
		//* points to terrasync binary in app bundle
		return QDir::currentPath().append("/fgx.app/Contents/MacOS/terrasync");
	}
	else if(runningOS() == LINUX){
		return QString("terrasync");
	}
	return QString("TODO - terrasync");
}

QString XSettings::terrasync_sync_path(){
	return value("terrasync_path").toString();
}	
	


//===========================================================================
//** temp
//===========================================================================
QString XSettings::temp_dir(){
	return QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).absolutePath();
}
QString XSettings::temp_dir(QString append_path){
	return temp_dir().append(append_path);
}


//===========================================================================
//** Save/Restore Window
//===========================================================================
void XSettings::saveWindow(QWidget *widget){
	setValue( _windowName(widget), QVariant(widget->saveGeometry()) );
}
void XSettings::restoreWindow(QWidget *widget){
	widget->restoreGeometry( value(_windowName(widget)).toByteArray() );
}
QString XSettings::_windowName(QWidget *widget){
	QString key_name = "window/";
	key_name.append(widget->property("settings_namespace").toString());
	key_name.append("/geometry");
	return key_name;
}

//===========================================================================
//** OS detection
//===========================================================================
int XSettings::runningOS() {
    #ifdef Q_WS_X11
		return XSettings::LINUX;
    #endif

    #ifdef Q_WS_MAC
		return XSettings::MAC;
    #endif

    #ifdef Q_WS_WIN
		return XSettings::WINDOWS;
    #endif

	return XSettings::UNKNOWN;
}

//===========================================================================
//** FGCom
//===========================================================================
QString XSettings::default_fgcom_no(){
	return QString("fgcom.flightgear.org.uk");
}
QString XSettings::default_fgcom_port(){
	return QString("16661");
}


QString XSettings::fgcom_exe_path(){
		if (runningOS() == MAC) {
			// Default fgcomx installation with fgcom installer by gral
			// Will change with fgcom coming in app bundle
			return QString("/usr/local/bin/fgcom");
		}
		else if(runningOS() == LINUX){
			return QString("fgcom");
		}
	
		else if(runningOS() == WINDOWS){
			return QString("fgcom"); // TODO!
		}
	
	return "fgcom";
}


//===========================================================================
//** SQLite Database
//===========================================================================
QString XSettings::db_file(){
	QString storedir = QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath();

	if(!QFile::exists(storedir)){
		QDir *dir = new QDir("");
		dir->mkpath(storedir);
	}
	return storedir.append("/fgx.sqlite.db");
}

QString XSettings::data_file(QString file_name){
	QString storedir = QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath();

	// create path is not exist
	if(!QFile::exists(storedir)){
		QDir *dir = new QDir("");
		dir->mkpath(storedir);
	}
	return storedir.append("/").append(file_name);
}


// using methods of osdab

void XSettings::uncompress(QString filename, QString destination)
{
	
	QString decompressedFileName;
	bool inflate(const QString& s);
	
	QString s(filename);
	
	QFile file(s);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug("Failed to open input file: %s", qPrintable(s));
		return;
	}
	
	// Output to a QByteArray....
	//
	// QByteArray decompressedData;
	// QDataStream dev(&decompressedData, QIODevice::WriteOnly);
	//
	// ...or to a QFile
	//
	//QFileInfo info(s);
	QString outFileName;
	//if (info.suffix() == "gz") {
	outFileName = destination;
	/*} else if (info.suffix() == "svgz") {
		outFileName = info.absoluteFilePath().left( info.absoluteFilePath().length() - 1 );
	} else {
		outFileName = info.absoluteFilePath().append(".decompressed");
	}*/
	// Quick and dirty :D
	int magik = 0;
	while (QFile::exists(outFileName)) {
		outFileName.append(QString(".%1").arg(++magik));
	}
	QFile out(outFileName);
	if (!out.open(QIODevice::WriteOnly)) {
		qDebug("Failed to open output file: %s", qPrintable(outFileName));
		return;
	}
	decompressedFileName = outFileName;
	QDataStream dev(&out);
	
	quint64 compressedSize = file.size();
	
	uInt rep = compressedSize / UNZIP_READ_BUFFER;
	uInt rem = compressedSize % UNZIP_READ_BUFFER;
	uInt cur = 0;
	
	// extract data
	qint64 read;
	quint64 tot = 0;
	
	char buffer1[UNZIP_READ_BUFFER];
	char buffer2[UNZIP_READ_BUFFER];
	
	/* Allocate inflate state */
	z_stream zstr;
	zstr.zalloc = Z_NULL;
	zstr.zfree = Z_NULL;
	zstr.opaque = Z_NULL;
	zstr.next_in = Z_NULL;
	zstr.avail_in = 0;
	
	int zret;
	
	/*
	 windowBits can also be greater than 15 for optional gzip decoding. Add
	 32 to windowBits to enable zlib and gzip decoding with automatic header
	 detection, or add 16 to decode only the gzip format (the zlib format will
	 return a Z_DATA_ERROR.  If a gzip stream is being decoded, strm->adler is
	 a crc32 instead of an adler32.
	 */
	if ( (zret = inflateInit2_(&zstr, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream))) != Z_OK ) {
		qDebug("Failed to initialize zlib");
		return;
	}
	
	int szDecomp;
	
	// Decompress until deflate stream ends or end of file
	do
	{
		read = file.read(buffer1, cur < rep ? UNZIP_READ_BUFFER : rem);
		if (read == 0)
			break;
		if (read < 0)
		{
			(void)inflateEnd(&zstr);
			qDebug("Read error");
			return;
		}
		
		cur++;
		tot += read;
		
		zstr.avail_in = (uInt) read;
		zstr.next_in = (Bytef*) buffer1;
		
		
		// Run inflate() on input until output buffer not full
		do {
			zstr.avail_out = UNZIP_READ_BUFFER;
			zstr.next_out = (Bytef*) buffer2;;
			
			zret = inflate(&zstr, Z_NO_FLUSH);
			
			switch (zret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&zstr);
					qDebug("zlib failed to decode file");
					return;
				default:
					;
			}
			
			szDecomp = UNZIP_READ_BUFFER - zstr.avail_out;
			if (dev.writeRawData(buffer2, szDecomp) != szDecomp)
			{
				inflateEnd(&zstr);
				qDebug("Write error");
				return;
			}
			
		} while (zstr.avail_out == 0);
		
	}
	while (zret != Z_STREAM_END);
	
	inflateEnd(&zstr);
	
	return;
	
}