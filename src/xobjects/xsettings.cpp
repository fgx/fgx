

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


/*! \class XSettings
 * \brief The XSettings is an extended QSetttings, but also includes most to the FGx configuration.
 *
 * The general idea of the class is to  provide one central place to access settings. It is
 * for this reason that methods are created to hide some of the completities, such as aircraft_path().
 *
 *
 * === Path Settings ===
 *
 * The essential path settings stored in the ini are:
 * - "fgfs_use_default" - whether to use the default installed fgfs binary
 * - "fgfs_custom_path" - the custom path to the fgfs executable
 * - "fgroot_use_default" - whether to use the default FG_ROOT path
 * - "fgroot_custom_path" - the custom path to fgdata/ eg a git checkout
 * - "terrasync_enabled" - whether terrasync is enabled
 * - "terrasync_path" - the path to the terrasync data directory
 *
 * The essential path settings abovar are written out on ConfirmPage of the SetupWizard
 */
/*!
  * @author: Peter Morgan
  * @author: Yves Sablonier
 */


XSettings::XSettings(QObject *parent) :
    QSettings(parent)
{
}

//===========================================================================
// Style
//===========================================================================
/** \return The currently selected style or default */
QString XSettings::style_current(){
	return value("gui_style", style_default()).toString();
}

/** \return The default style for platform */
QString XSettings::style_default(){
	if(runningOs() == MAC){
		return QString("Macintosh (aqua)");
	}
	if(runningOs() == WINDOWS){
		return QString("cleanlooks");
	}
	if(runningOs() == LINUX){
		return QString("cleanlooks");
	}
	return QString("cleanlooks");
}

//===========================================================================
//** fgx Install path
//===========================================================================
QString XSettings::fgx_path(){
	return QDir::currentPath().append("/fgx.app/Contents/MacOS");
}


//===========================================================================
//== fgfs Executable
//===========================================================================
/** \brief Path to FlightGear Executable
 *
 * Returns the path to the fg executable. If the default path
 * is selected, then that is returned; otherwise the custom selected executable
 * \return The absolute path.
 */
QString XSettings::fgfs_path(){
	if(fgfs_use_default()){
		return fgfs_default_path();
	}
	return value("fgfs_custom_path", "").toString();
}
bool XSettings::fgfs_use_default(){
	return value("fgfs_use_default", '1').toBool();
}

/** \brief Platform specific default path for the fgfs executable
 *
 */
QString XSettings::fgfs_default_path(){

	if(runningOs() == MAC){
		return QDir::currentPath().append("/fgx.app/Contents/MacOS/fgfs");

	}else if(runningOs() == LINUX){
		return QString("/usr/local/bin/fgfs");


	}else if(runningOs() == WINDOWS){
		return QString("C:/Program Files/FlightGear/bin/win32/fgfs.exe");
	}

	return QString("UNKNOW OS in default_fgfs_path()");
}

//===========================================================================
//== fgroot  path
//===========================================================================
/** \return The absolute path to FG_ROOT ie /fgdata directory */
QString XSettings::fgroot(){
	if(fgroot_use_default()){
		return fgroot_default_path();
	}
	return value("fgroot_custom_path", "").toString();
}
bool XSettings::fgroot_use_default(){
	return value("fgroot_use_default", '1').toBool();
}

/** \brief Path to FG_ROOT with appended path
 *
 * Returns the path to the FG_ROOT. If the default install
 * is selected, then that is returned, otherwise the custom selected fg_data path.
 * The file is appended with the append_path
 * \return The absolute path.
 */

QString XSettings::fgroot(QString append_path){
	return this->fgroot().append(append_path);
}

/** \brief Platform specific default path for the FG_ROOT dir
 *
  * \return The absolute path to FG_ROOT
 */
QString XSettings::fgroot_default_path(){

	if(runningOs() == MAC){
		return QDir::currentPath().append("/fgx.app/Contents/Resources/fgx-fgdata");

	}else if(runningOs() == LINUX){
		return QString("/usr/share/games/FlightGear");

	}else if(runningOs() == WINDOWS){
		return QString("C:/Program Files/FlightGear/data");
	}

	return QString("OS NOT HANDLED in default_fg_root");
}

//===========================================================================
//** Paths Sane
//===========================================================================
/** \brief Checks whether the executablem FG_ROOT paths are sane.
 *
 * \return true if sane
 */
bool XSettings::paths_sane(){
	if(!QFile::exists(fgfs_path())){
		return false;
	}
	if(!QFile::exists(fgroot())){
		return false;
	}
	return true;
}


//===========================================================================
//** Aircraft Path
//===========================================================================
/** \brief The path to the /Aircraft directory
 *
  * \return absolute path.
 */
QString XSettings::aircraft_path(){
	return fgroot().append("/Aircraft");
	outLog("*** FGx settings: Aircraft path: " + fgroot().append("/Aircraft") + " ***");
}
/** \brief Path to the /Aircraft directory with a dir appended.
 *
 * \return absolute path.
 */
QString XSettings::aircraft_path(QString dir){
	return fgroot().append("/Aircraft/").append(dir);
	outLog("*** FGx settings: Aircraft path: " + fgroot().append("/Aircraft/").append(dir) + " ***");
}


//===========================================================================
//** Airports path
//===========================================================================
/** \brief Return the absolute path to the /Airports directory
 *
 * \return If TerraSync is enabled, then returns a terrasync folder, otherwise the default.
 */
QString XSettings::airports_path(){

	//* Using terrasync
	if(terrasync_enabled()){
		if(runningOs() == MAC){
			QString terrascenehome(QDir::homePath());
			terrascenehome.append("/Documents/TerrasyncScenery");
			return terrascenehome;
			outLog("*** FGx settings: Airports path: " + terrascenehome + " ***");
		}else{
			//* Use the terra sync path
			return terrasync_sync_data_path().append("/Airports");
			outLog("*** FGx settings: Airports path: " + terrasync_sync_data_path().append("/Airports") + " ***");
		}
	}
	//* Otherwise return the FG_ROOT airports/
	return fgroot().append("/Scenery/Airports");
	outLog("*** FGx settings: Airports path: " + fgroot().append("/Scenery/Airports") + " ***");
}

//** Apt Dat
/** \brief Return the absolute path to the /apt.dat.gz file
 *
 * The file is uncompress to the temp_dir()/apt.dat is a gz is found
 * \return The absolute path to unpacked apt.dat
 */
QString XSettings::apt_dat_file(){
	QString aptdatloc(QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath().append("/apt.dat"));
	if( QFile::exists(aptdatloc) == false){
		uncompress(fgroot("/Airports/apt.dat.gz"), QString(QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath().append("/apt.dat")));
		outLog(QString(temp_dir().append("apt.dat")));
	}
	return aptdatloc;
}

//===========================================================================
//** Scenery Path - TODO x/win
//===========================================================================
/** \brief Returns the absolute path to the /Scenery file
 *
 * \return The absolute path.
 */
QString XSettings::scenery_path(){

	return fgroot("/Scenery");
}

//===========================================================================
//** TerraSync
//===========================================================================
/** \brief  Using terrasync for scenery
 *
 * \return true if using terrasync
 */
bool XSettings::terrasync_enabled(){
	return value("terrasync_enabled").toBool();
}
/** \brief terrasync executable
 *
  * \return path to terrasync executable
  * \todo Windows path
 */
QString XSettings::terrasync_exe_path(){
	if (runningOs() == MAC) {
		//* points to terrasync binary in app bundle
		return QDir::currentPath().append("/fgx.app/Contents/MacOS/terrasync");

	}else if(runningOs() == LINUX){
		return QString("terrasync");

	}else if(runningOs() == WINDOWS){
		return QString("TODO/path to terrasync.exe");
	}
	return QString("TODO - terrasync");
}
/** \brief terrasync data path
 *
  * \return path to where terrasync will store files downloaded.
 */
QString XSettings::terrasync_sync_data_path(){
	return value("terrasync_path").toString();
}	
	


//===========================================================================
//** temp
//===========================================================================
/** \brief location if temp directoty , os specific
 *
 * Shortcut method for Qt's storageLocation()
 * \return Absolute path
 */
QString XSettings::temp_dir(){
	return QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).absolutePath();
}
/** \brief location if temp directoty , os specific with appended file/path
 *
 * Shortcut method for Qt's storageLocation()
 * \return Absolute path with appended paths.
 */
QString XSettings::temp_dir(QString append_path){
	return temp_dir().append(append_path);
}


//===========================================================================
//** Save/Restore Window
//===========================================================================
/** \brief Saves a window position
 *
 */
void XSettings::saveWindow(QWidget *widget){
	setValue( _windowName(widget), QVariant(widget->saveGeometry()) );
}
/** \brief Restores a window position
 *
 */
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
/** \brief What OS is running
 *
 * \return a OS enum value
 */
int XSettings::runningOs() {
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
		if (runningOs() == MAC) {
			// Default fgcomx installation with fgcom installer by gral
			// Will change with fgcom coming in app bundle
			return QString("/usr/local/bin/fgcom");
		}
		else if(runningOs() == LINUX){
			return QString("fgcom");
		}
	
		else if(runningOs() == WINDOWS){
			return QString("fgcom"); // TODO!
		}
	
	return "fgcom";
}


//===========================================================================
//** Data File eg airports.txt
//===========================================================================
/** \brief Path to a data file eg data_file("airports.txt")
 *
 * \return Absolute path to the file
 */
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

/** \brief Log File
 *
 * \return Absolute path to log file
 */
QString XSettings::log_file_path(){
	if(runningOs() == WINDOWS){
		return temp_dir("/fgx-log.txt");

	}else if(runningOs() == MAC){
		return QDir::homePath().append("/Library/Logs/fgx.log");

	}else if(runningOs() == LINUX){
		return temp_dir("/fgx.log");

	}else{
		return "UNKNOWN log_file_path()";
	}
}
