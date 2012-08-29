/* ********************************************************
   *  utilities.cpp
   *
   *  Created by Geoff R. Mclane, Paris
   *  (c) 2011 GPL2 (or later)
   *
   ******************************************************** */

#include <QMutex>
#include "utilities.h"
#include "fileDialog.h"
#include "dirDialog.h"
#include "xobjects/mainobject.h"


/*
	LOG FILE FUNCTIONS
 */

static QString m_logFile;
static QMutex m_logMutex;

// set log file name
void util_setLogFile(QString file)
{
	m_logFile = file; // set name
}

// create a log file and set name
bool util_createLogFile(QString file)
{
	bool bret = false;
	QFile data(file);
	if (data.open(QIODevice::ReadWrite | QIODevice::Truncate | QFile::Text)) {
		QTextStream out(&data);
		QString dt = util_getDateTimestg();
		out << dt << " - ";
		out.flush();
		data.close();
		bret = true;    // log is ok
		util_setLogFile(file); // set name
	}
	if (bret)
		outLog("Commenced log");
	return bret;
}

bool util_setStdLogFile()
{
	/*
	QString dir;
	QString file;
	QString log;

#if defined(Q_OS_WIN)
	dir  = "C:/Windows/Temp";
	file = "fgx-log.txt";
#elif defined(Q_OS_MAC)
   dir.append(QDir::homePath());
   dir.append("/Library/Logs");
   file = "fgx.log";
#elif defined(Q_OS_UNIX)
   dir  = "/tmp";
   file = "fgx.log";
#else
#error Unknown OS - Add default log dir and file for this OS!
#endif
   log = dir + "/" + file;
   */
	return util_createLogFile(MainObject::log_file_path());
}

// append to LOG file, after acquiring the mutext
void outLog(QString s, int flag)
{
	if (m_logFile.size()) {
		QFile data(m_logFile);
		m_logMutex.lock();  // grap the mutex
		if (data.open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
			QTextStream out(&data);
			if (flag & olflg_AddTm)
				out << util_getTimestg()+" - ";
			out << s;
			if (flag & olflg_AddLE)
				out << "\n";
			data.close(); // ensure it get to disk
		}
		m_logMutex.unlock(); // release the mutex
	}
}

/*
	Utility function that recursively searches, if desired, for files per filters.
*/
QStringList findFiles(const QString &startDir, QStringList filters, bool recurse)
{
	QStringList names;
	QDir dir(startDir);

	// 1: Get the files matching the filter
	foreach (QString file, dir.entryList(filters, QDir::Files))
		names += startDir + "/" + file;

	if (recurse) {
		// 2: If recursive, get ALL directories, and try for more files
		foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
			names += findFiles(startDir + "/" + subdir, filters, recurse);
	}
	return names;
}

QStringList findDirs(QString startDir, bool recurse)
{
    QStringList names;
    QDir dir(startDir);
    foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        QString ndir(startDir+"/"+subdir);
        names += ndir;
        if (recurse)
            names += findDirs(ndir,recurse);
    }
    return names;
}

// given millisecond, return appropriate (nice) string, with units
QString getElapTimeStg(int ms)
{
	QString fmt = "";
	if (ms < 1000) {
		fmt.sprintf("%d ms", ms);
	} else {
		int secs = ms / 1000;
		ms = ms % 1000;
		if (secs < 60) {
			if (ms)
				fmt.sprintf("%d.%03d secs", secs, ms);
			else
				fmt.sprintf("%d secs", secs);
		} else {
			int mins = secs / 60;
			secs = secs % 60;
			if (mins < 60) {
				if (ms)
					fmt.sprintf("%d:%02d.%03d mins", mins, secs, ms);
				else if (secs)
					fmt.sprintf("%d:%02d mins", mins, secs);
				else
					fmt.sprintf("%d mins", mins);
			} else {
				int hrs = mins / 60;
				mins = mins % 60;
				if (ms)
					fmt.sprintf("%d:%02d:%02d.%03d hrs", hrs, mins, secs, ms);
				else if (secs)
					fmt.sprintf("%d:%02d:%02d hrs", hrs, mins, secs);
				else if (mins)
					fmt.sprintf("%d:%02d hrs", hrs, mins);
				else
					fmt.sprintf("%d hrs", hrs);
			}
		}
	}
	return fmt;
}

QString util_getDateTimestg(void)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString dt = datetime.toString("yyyy/MM/dd HH:mm:ss");
	return dt;
}
QString util_getTimestg(void)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString dt = datetime.toString("HH:mm:ss");
	return dt;
}




//===========================================================================================
//== IACO Radio Comms Phonetic
//===========================================================================================
//**  cut and plaster by pete from email from Geoff GBH


typedef struct tagABC {
 char abc;
 char * morse;
 char * telephony;
 char * phonetic;
}ABC, * PABC;

static ABC abc[] = {
 { 'A', (char *)".-", (char *)"Alfa", (char *)"AL-FAH" },
 { 'B', (char *)"-...", (char *)"Bravo", (char *)"BRAH-VOH" },
 { 'C', (char *)"-.-.", (char *)"Charlie", (char *)"CHAR-LEE" },
 { 'D', (char *)"-..", (char *)"Delta", (char *)"DELL-TAH" },
 { 'E', (char *)".", (char *)"Echo", (char *)"ECK-OH" },
 { 'F', (char *)"..-.", (char *)"Foxtrot", (char *)"FOKS-TROT" },
 { 'G', (char *)"--.", (char *)"Golf", (char *)"GOLF" },
 { 'H', (char *)"....", (char *)"Hotel", (char *)"HOH-TEL" },
 { 'I', (char *)"..", (char *)"India", (char *)"IN-DEE-AH" },
 { 'J', (char *)".---", (char *)"Juliet", (char *)"JEW-LEE-ETT" },
 { 'K', (char *)"-.-", (char *)"Kilo", (char *)"KEY-LOH" },
 { 'L', (char *)".-..", (char *)"Lima", (char *)"LEE-MAH" },
 { 'M', (char *)"--", (char *)"Mike", (char *)"MIKE" },
 { 'N', (char *)"-.", (char *)"November", (char *)"NO-VEM-BER" },
 { 'O', (char *)"---", (char *)"Oscar", (char *)"OSS-CAH" },
 { 'P', (char *)".--.", (char *)"Papa", (char *)"PAH-PAH" },
 { 'Q', (char *)"--.-", (char *)"Quebec", (char *)"KEH-BECK" },
 { 'R', (char *)".-.", (char *)"Romeo", (char *)"ROW-ME-OH" },
 { 'S', (char *)"...", (char *)"Sierra", (char *)"SEE-AIR-RAH" },
 { 'T', (char *)"-", (char *)"Tango", (char *)"TANG-GO" },
 { 'U', (char *)"..-", (char *)"Uniform", (char *)"YOU-NEE-FORM" },
 { 'V', (char *)"...-", (char *)"Victor", (char *)"VIK-TAH" },
 { 'W', (char *)".--", (char *)"Whiskey", (char *)"WISS-KEY" },
 { 'X', (char *)"-..-", (char *)"Xray", (char *)"ECKS-RAY" },
 { 'Y', (char *)"-..-", (char *)"Yankee", (char *)"YANG-KEY" },
 { 'Z', (char *)"--..", (char *)"Zulu", (char *)"ZOO-LOO" },
 { '1', (char *)".----", (char *)"One", (char *)"WUN" },
 { '2', (char *)"..---", (char *)"Two", (char *)"TOO" },
 { '3', (char *)"...--", (char *)"Three", (char *)"TREE" },
 { '4', (char *)"....-", (char *)"Four", (char *)"FOW-ER" },
 { '5', (char *)".....", (char *)"Five", (char *)"FIFE" },
 { '6', (char *)"-....", (char *)"Six", (char *)"SIX" },
 { '7', (char *)"--...", (char *)"Seven", (char *)"SEV-EN" },
 { '8', (char *)"---..", (char *)"Eight", (char *)"AIT" },
 { '9', (char *)"----.", (char *)"Nine", (char *)"NIN-ER" },
 { '0', (char *)"-----", (char *)"Zero", (char *)"ZEE-RO" },
 { 0, 0, 0, 0 }
};

char * abc_to_morse(char ch)
{
   PABC pabc = &abc[0];
   while (pabc->morse) {
	   if (ch == pabc->abc)
		   return pabc->morse;
	   pabc++;
   }
   return 0;
}
char * abc_to_telephony(char ch)
{
   PABC pabc = &abc[0];
   while (pabc->morse) {
	   if (ch == pabc->abc)
		   return pabc->telephony;
	   pabc++;
   }
   return 0;
}
char * abc_to_phonetic(char ch)
{
   PABC pabc = &abc[0];
   while (pabc->morse) {
	   if (ch == pabc->abc)
		   return pabc->phonetic;
	   pabc++;
   }
   return 0;
}

char telephony_to_abc(char * pch)
{
   PABC pabc = &abc[0];
   while (pabc->morse) {
	   if (strcmp(pch,pabc->telephony) == 0)
		   return pabc->abc;
	   pabc++;
   }
   return 0;
}
char *telephony_to_morse(char * pch)
{
   PABC pabc = &abc[0];
   while (pabc->morse) {
	   if (strcmp(pch,pabc->telephony) == 0)
		   return pabc->morse;
	   pabc++;
   }
   return 0;
}
char *telephony_to_phonetic(char * pch)
{
   PABC pabc = &abc[0];
   while (pabc->morse) {
	   if (strcmp(pch,pabc->telephony) == 0)
		   return pabc->phonetic;
	   pabc++;
   }
   return 0;
}

// =================================
// testing an exe
void testExe::init()
{
    timeOut = 3; // default to 3 seconds
    result = -3;
    // really, these have alread been done
    exe.clear();
    args.clear();
    finds.clear();
    extraEnv.clear();
    rtDir.clear();
}

testExe::testExe()
{
    init();
}
testExe::testExe(QString ex)
{
    init();
    exe = ex; // set the EXE
}
testExe::~testExe()
{
    // nothing allocated, so nothing to do...
}
bool testExe::runTest()
{
    result = false;
    QTime rt;
    int i, max, cnt, ind;
    errExit = -2;
    runTime = 0;
    if (exe.length() == 0) {
        stgResult = "FAILED: No exe set!";
        return result;
    }
    QProcess proc;
    QString all, tmp;
    rt.start();
    if (rtDir.length()) {
        //= set the specific runtime directory
        proc.setWorkingDirectory(rtDir);
    }
    if (extraEnv.size()) {
        //= append new env vars
        QStringList env = QProcess::systemEnvironment();
        env << extraEnv;
        proc.setEnvironment(env);
    }
    QString arguments = exe;
    int ms = timeOut * 1000;
    if (ms <= 0)
        ms = 10; // minimum of 10 ms wait
    if (args.size())
        arguments += " "+args.join(" ");
    // try the running of the process
    proc.start(arguments, QIODevice::ReadWrite);
    if (proc.waitForStarted()) {
        proc.waitForFinished(ms);
        errExit = proc.exitCode();
        outStd = proc.readAllStandardOutput();
        outErr = proc.readAllStandardError();
        all = outStd + outErr;
        max = finds.size();
        if (max) {
            // must find these string in the result
            cnt = 0;
            for (i = 0; i < max; i++) {
                tmp = finds.at(i);
                ind = all.indexOf(tmp);
                if (ind >= 0)
                    cnt++;
            }
            if (cnt == max) { // found all required finds
                stgResult = "SUCCESS: All finds found";
                result = true;
            } else {
                stgResult.sprintf("FAILED: Found %d of %d in output",
                                  cnt, max);
            }
        } else {
            // no finds to find - base result on exit code
            if (errExit == 0) {
                stgResult = "SUCCESS: Exit code is ZERO";
                result = true;
            } else {
                stgResult.sprintf("FAILED: Exit code is %d",errExit);
            }
        }
    } else {
        stgResult = "FAILED: NO start ["+arguments+"]";
    }
    runTime = rt.elapsed();
    return result;
}
// =================================

// replacement getFileName function, while my Qt/Ubuntu built-in function FAILS!!!
// #include "fileDialog.h"
QString util_getFileName(QWidget *parent, QString title, QString previous, QStringList filt)
{
    fileDialog d(parent);           // instantiate dialog
    d.init(title,previous,filt);    // do setup
    d.exec();                       // run dialog
    QString newFile = d.getFileName();  // get results
    return newFile;                 // return results - "" if user cancelled
}

QString util_getDirName(QWidget *parent, QString title, QString previous)
{
    dirDialog d(parent);           // instantiate dialog
    d.init(title,previous);        // do setup
    d.exec();                      // run dialog
    QString newFile = d.getDirName();  // get results
    return newFile;                 // return results - "" if user cancelled
}


QString util_getCurrentWorkDirectory()
{
    return QDir::currentPath();
}

QString util_getBasePath(QString in)
{
    QString path = in;
    util_ensureUnixPathSep(path);
    int ind = path.lastIndexOf(QChar('/'));
    if (ind >= 0)
        path = path.left(ind + 1);
    else
        path = "";
    return path;
}

QString util_getBaseName(QString in)
{
    QString path = in;
    util_ensureUnixPathSep(path);
    int ind = path.lastIndexOf(QChar('/'));
    int len = path.length();
    if ((ind > 0)&&(len > ind))
        path = path.right(len-ind-1);
    return path;
}


bool util_ensureUnixPathSep(QString &txt)
{
    int ind = txt.indexOf(QChar('\\'));
    if ( ind >= 0 ) {
        txt.replace("\\","/"); // forced use of ONLY '/' char
        return true;
    }
    return false;
}

// =====================
// native get directory, file and new file
QString util_browseDirectory(QWidget * parent, QString prompt, QString current)
{
    QString filePath =
        QFileDialog::getExistingDirectory(
                parent, prompt, current,
                QFileDialog::ShowDirsOnly);
    return filePath;
}

QFileDialog::Options _getUserOptions(int options)
{
    QFileDialog::Options opts = 0;
    if (options & 0x0001)
        opts |= QFileDialog::ShowDirsOnly;
    if (options & 0x0002)
        opts |= QFileDialog::DontResolveSymlinks;
    if (options & 0x0004)
        opts |= QFileDialog::DontConfirmOverwrite;
    if (options & 0x0008)
        opts |= QFileDialog::DontUseSheet;
    if (options & 0x0010)
        opts |= QFileDialog::DontUseNativeDialog;
    if (options & 0x0020)
        opts |= QFileDialog::ReadOnly;
    if (options & 0x0040)
        opts |= QFileDialog::HideNameFilterDetails;
    return opts;
}

QString util_browseFile(QWidget * parent, QString prompt, QString current,
                        QStringList filter, int options)
{
    QString filt;
    if (filter.count()) {
        filt = "Files (";
        for (int i = 0; i < filter.count(); i++) {
            if (i)
                filt.append(" ");
            filt.append(filter.at(i));
        }
        filt.append(")");
    }
    QFileDialog::Options opts = _getUserOptions(options);
    QString filePath =
        QFileDialog::getOpenFileName(
                parent, prompt, current, filt, 0, opts );
    return filePath;
}

QString util_browseNewFile(QWidget * parent, QString prompt, QString current,
                           QStringList filter, int options)
{
    QString filt;
    if (filter.count()) {
        filt = "Files (";
        for (int i = 0; i < filter.count(); i++) {
            if (i)
                filt.append(" ");
            filt.append(filter.at(i));
        }
        filt.append(")");
    }
    QFileDialog::Options opts = _getUserOptions(options);
    QString filePath =
        QFileDialog::getSaveFileName(
                parent, prompt, current, filt, 0, opts );
    return filePath;
}

// FIX20110823 - dist_est_km() returned 'nan' if virtually the same point
// double dist_est_km(double lat1, double lon1, double lat2, double lon2)
double util_DistEst_km(double lat1, double lon1, double lat2, double lon2)
{
    double d, dd;
    double small = 0.0000001;
    double dt = fabs(lat1 - lat2);
    double dn = fabs(lon1 - lon2);
    if ((dt < small)&&(dn < small))
        return 0.0; // this is to try to avoid a 'nan'
    dt = lat1 * DEG2RAD;
    dn = lat2 * DEG2RAD;
    dd = (lon2 - lon1) * DEG2RAD;
    d = sin(dt) * sin(dn);
    d += cos(dt) * cos(dn) * cos(dd);
    return (acos(d) * EARTH_RAD);
}

// eof - utilities.cpp
