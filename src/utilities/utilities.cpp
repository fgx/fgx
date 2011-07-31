/* ********************************************************
   *  utilities.cpp
   *  for fgx
   *
   *  Created by Geoff R. Mclane, Paris
   *  © 2011 GPL2 (or later)
   *
   ******************************************************** */

#include <QMutex>
#include "utilities.h"
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

// eof - utilities.cpp
