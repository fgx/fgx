/* ********************************************************
   *  loadAptDat
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Dec 2011 GPL2 (or later)
   *
   *  Load the flightgear apt.dat.gz file
   *  See: http://data.x-plane.com/file_specs/Apt715.htm
   *  CODES
   *  1 	Airport header data.
   * 16 	Seaplane base header data. No airport buildings or boundary fences will be rendered in X-Plane.
   * 17 	Heliport header data.  No airport buildings or boundary fences will be rendered in X-Plane.
   * 10 	Runway or taxiway at an airport.
   * 14 	Tower view location. (850 - Airport viewpoint One or none for each airport)
   * 15 	Ramp startup position(s)
   * 18 	Airport light beacons (usually "rotating beacons" in the USA).  Different colours may be defined.
   * 19 	Airport windsocks. one or many for each airport
   * 50 to 56 	Airport ATC (Air Traffic Control) frequencies.
   *
   * 850 (ADDITIONAL/ALTERNATIVE) CODES
   * 100  Runway
   * 101  Water runway
   * 102  Helipad
   * 110  Pavement (taxiway or ramp) header   Must form a closed loop
   * 120  Linear feature (painted line or light string) Can form closed loop
   * 130  Airport boundary header Must form a closed loop
   * 111  Node All nodes can also include a style (line or lights)
   * 112  Node with Bezier control point Bezier control points define smooth curves
   * 113  Node with implicit close of loop Implied join to first node in chain
   * 114  Node with Bezier control point, with implicit close of loop Implied join to
   *      first node in chain
   * 115  Node terminating a string (no close loop) No styles used
   * 116  Node with Bezier control point, terminating a string (no close loop) No
   *      styles used
   *  20  Taxiway sign (inc. dist-remain) Zero, one or many for each airport
   *  21  Lighting (VASI, PAPI, Wig-Wag, etc.) Zero or many for each airport
   *
   *
   * 0 = 1 	Identifies this as an airport header line.  Note that a code 16 will identify this as a seaplane/floatplane base, and a code 17 will identify it as a heliport.
   * 1 = 5355 	Airport elevation (in feet above MSL).
   * 2 = 1 	Airport has a control tower (1=yes, 0=no).
   * 3 = 0 	Display X-Plane’s default airport buildings (1=yes, 0=no).  This is usually ‘no’ if any custom taxiways have been defined.
   * 4 = KABQ 	Identifying  code for the airport (the ICAO code, if one exists).   It is helpful if these are unique, but this may not be possible in all cases.
   * 5+= Albuquerque Intl Sunport 	Airport name.
   *
   *  0 = 10 	runway or taxiway segment.
   *  1 = 35.044209 	Latitude (in decimal degrees) of runway or taxiway segment center.
   *  2 = -106.598557 	Longitude (in decimal degrees) of runway or taxiway segment center.
   *  3 = 08x 	Runway number (eg “25x” or “24R”).  If there is no runway suffix (eg. “L”, “R”, “C” or "S"), then an “x” is used.  “xxx” identifies the entry as a taxiway.  Helipads at the same airport are numbered sequentially as "H1x", H2x".
   *  4 = 90.439 	True (not magnetic) heading of the runway in degrees.  Must be between 0.00 and 360.00.
   *  5 = 13749 	Runway or taxiway segment length in feet.
   *  6 = 1000.0000 	Length of displaced threshold (1,000 feet) for runway 08 and for the reciprocal runway 26 (0 feet).  The length of the reciprocal runway’s displaced threshold is expressed as the fractional part of this number.  Take the runway 26 displaced threshold length  (in feet) and divide it by 10,000, then add it to the displaced threshold length for runway 08.  For example, for displaced threshold lengths of 543 feet and 1234 feet, the code would be 543.1234.
   *  7 = 0.1000 	Length of stopway/blastpad/over-run at the approach end of runway 08 (0 feet) and for runway 26 (1,000 feet), using the same coding structure defined above.  FYI, in the real world the stopway/blastpad/over-run is usually marked with large yellow chevrons, and aeroplane movements are not permitted.
   *  8 = 150 	Runway or taxiway segment width in feet.
   *  9 = 252231 Runway or taxiway segment lighting codes.
   * 10 = 02 	Runway or taxiway surface code for the runway or taxiway segment.
   * 11 = 0 	Runway shoulder code
   * 12 = 3 	Runway markings
   * 13 = 0.25 	Runway smoothness.
   * 14 = 1 	Runway has 'distance remaining' signs
   *
   *  0 = 15 	Identifies this as a data line for an airport startup location (code 15). Multiple allowed
   *  1 = 35.047215 	Latitude (in decimal degrees) of the viewpoint or startup location.
   *  2 = -106.618576 	Longitude (in decimal degrees) of the viewpoint or startup location.
   *  3 = 0.00 	Elevation of the viewpoint, or the heading of the aeroplane when placed at the startup location.
   *  4 = Gate B1 (American Airlines)   Name
   *
   *  0 = 18 	Identifies this as a data line for an airport light beacon (code 18).
   *  1 = 35.045031 	Latitude (in decimal degrees) of the light beacon.
   *  2 = -106.598549 	Longitude (in decimal degrees) of the light beacon.
   *  3 = 1 	Identifies the colours of the light beacon.  Here code 1 implies a standard white-green flashing light.  Options are:
   *  4 = Light beacon 	Name for this light beacon (not used by X-Plane).
   *
   *  0 = 19 	Identifies this row as an airport windsock (code 19).
   *  1 = 35.045176 	Latitude (in decimal degrees) of the airport windsock.
   *  2 = -106.621581 	Longitude (in decimal degrees) of the airport windsock.
   *  3 = 1 	Windsock lighting (1=illuminated, 0=not illuminated).
   *  4 = Windsock 	Name for this windsock (not used by X-Plane).
   *
   *  0 = 14 	Identifies this as a data line for a tower viewpoint (code 14).Only a single tower viewpoint is permitted.
   *  1 = 35.047005 	Latitude (in decimal degrees) of the viewpoint.
   *  2 = -106.608162 	Longitude (in decimal degrees) of the viewpoint.
   *  3 = 100 	Height (in feet) above ground level of viewpoint.
   *  4 = 1 	Flag to indicate if a control tower object should be drawn at this location in X-Plane.  0=no tower, 1=draw tower.
   *  6 - tower viewpoint 	Name of this viewpoint
   *
   *  0 = 53 	Identifies this as an airport ATC frequency line.  Codes in the 50 - 59 range are used to identity different ATC types.
   *  1 = 12190 	Airport ATC frequency, in Megahertz multiplied by 100 (ie. 121.90 MHz in this example).
   *  2 = GND 	Name of the ATC frequency.  This is often an abbreviation (such as GND for "Ground").
   *
   * 850 DATA
   * CODE
   *
   *
   ******************************************************** */

#include "loadAptDat.h"
#include "utilities/utilities.h"
#include "utilities/simgear/SGGeodesy.h"
#include "utilities/simgear/constants.h"

#define LATLONOK(a,b) ((a >= -90.0) && (a <= 90.0) && (b >= -180.0) && (b <= 180.0))

loadAptDat::loadAptDat(QObject *par)
{
    Q_UNUSED(par);
    pAirList = new AIRPORTLIST;
    loadTime_ms = 0;
    workthread = 0;
    loadItem.in_loading = false;
    last_load = "";      // last apt.dat loaded = NONE yet

}

loadAptDat::~loadAptDat()
{
    if (workthread) {
        loadItem.work.abort = true;   // set abort
        if (workthread->in_function) {
            // need to wait a bit
            int msecs = 0;  // wait for MAX 5 seconds
            QTime tm;
            tm.start();
            while (msecs < 5000) {
                msecs += 50;
                SleeperThread::msleep(50); // give up CPU
                if (!workthread->in_function)
                    break;
            }
            if (workthread->in_function) {
                outLog("loadAptdat: Workthread no aborted after "+getElapTimeStg(tm.elapsed())+", using 'terminate'!");
                workthread->was_terminated = true;
                workthread->terminate();
            } else {
                outLog("loadAptDat: Workthread aborted on request after "+getElapTimeStg(tm.elapsed()));
            }
        }
        delete workthread;
    }
    clear_list();
}

PAIRPORTLIST loadAptDat::setAirListPtr(PAIRPORTLIST pal)
{
    PAIRPORTLIST cpal = getAirListPtr();
    pAirList = pal;
    return cpal;
}
PAIRPORTLIST loadAptDat::getAirListPtr()
{
    return pAirList;
}

/* ----------------------------------------------
   isFileLoaded:
   Input: QString valid, existing file.
   Return: true if name, size and date match
           false if not
   ---------------------------------------------- */
bool loadAptDat::isFileLoaded(QString zf)
{
    QFile file(zf);
    QFileInfo fi(file);
    if (last_load.length()) {
        // check if this is a NEW file
        if ((zf == last_load) &&
            (fi.size() == last_size) &&
            (fi.lastModified() == last_date)) {
            // YEEK, this is what is already loaded!
            return true;
        }
    }
    return false;
}

bool loadAptDat::loadDirect(QString zf, int flag)
{
    if (loadItem.in_loading)
        return false;
    loadItem.work.abort = false;
    loadItem.zf = zf;
    loadItem.pAirList = getAirListPtr();
    loadItem.optionFlag = flag;     // like lf_noAddTax;
    loadItem.in_loading = true;
    _loadStatic(&loadItem);
    loadItem.in_loading = false;
    outLog(loadItem.msg);
    loadTime_ms = loadItem.loadTime_ms;
    return !(loadItem.result); // RETURN SUCCESS
}

// ret 0 = on thread, flag = one or more of the flags - default none
int loadAptDat::loadOnThread(QString zf, int flag)
{
    if (loadItem.in_loading)
        return 1; // 1 = already running - do not start up a 2nd time
    QFile file(zf);
    if (!file.exists())
        return 2; // 2 = no file exist - do not fire up the thread
    if (!(flag & lf_ForceLoad) && isFileLoaded(zf)) {
        return 3; // 3 = already loaded, and no FORCE applied!
    }
    // we are going threading - setup the thread parameters
    loadItem.in_loading = true;
    loadItem.zf = zf;
    loadItem.optionFlag = flag;     // like lf_noAddTax;
    loadItem.work.user_type = tht_loadAptDat;
    loadItem.work.act = _loadStatic;  // (static) loader
    loadItem.work.vp = &loadItem;
    loadItem.work.abort = false;
    loadItem.pAirList = getAirListPtr();
    if (!workthread)
        workthread = new workThread;
    // going for the LOAD, store the file information
    QFileInfo fi(file);
    last_load = zf;
    last_size = fi.size();
    last_date = fi.lastModified();
    connect(workthread,SIGNAL(work_done(int,int)),this,SLOT(thread_done(int,int)));
    loadItem.threadnum = workthread->work(_loadStatic,&loadItem);   // start up the thread to do the work
    return 0;   // no problems, thread has started...
}

// connected to workThread::work_done(int num, int ms) signal
void loadAptDat::thread_done(int wn,int ms)
{
    loadItem.in_loading = false;
    QString msg;
    outLog(loadItem.msg);
    msg.sprintf("Done work %d, in ",wn);
    msg.append(getElapTimeStg(ms));
    loadTime_ms = ms;
    outLog(msg);
    emit load_done();   // signal DONE work thread
}

void loadAptDat::clear_list(PAIRPORTLIST pal)
{
    if (!pal)
        pal = getAirListPtr();
    if (pal) {
        int i, max;
        max = pal->count();
        for (i = 0; i < max; i++) {
            PAD_AIRPORT pad = pal->at(i);
            delete pad;
        }
        pal->clear();
    }
}

int loadAptDat::getAirportCount()
{
    AIRPORTLIST *pal = getAirListPtr();
    int max = pal->count();
    return max;
}
int loadAptDat::getRunwayCount()
{
    AIRPORTLIST *pal = getAirListPtr();
    int i, max;
    int count = 0;

    max = pal->count();
    for (i = 0; i < max; i++) {
        PAD_AIRPORT pad = pal->at(i);
        count += pad->run_list.count();
    }
    return count;
}

static int debug_stop(int i) {
    int j = i;
    int k = j / 2;
    return k;
}

/* ================================
   static void loadAptDat::_loadStatic(void * vp)
   This 'static' member can be called from a thread
   or 'directly'
   ================================ */
void loadAptDat::_loadStatic(void * vp)
{
    PLOADITEM pli = (PLOADITEM)vp;
    QString zf(pli->zf);
    AIRPORTLIST *pal = pli->pAirList;
    // switch to negative flags, so 0 is the default
    bool add_taxiways = !(pli->optionFlag & lf_noAddTax);
    bool skip_outofrange = !(pli->optionFlag & lf_noSkipOOR);
    bool fix_name = !(pli->optionFlag & lf_noFixName);
    pli->result = false; // set FAILED
    QTime tm;
    QString msg;
    fgx_gzHandle gzf; // opaque file gz handle
    QFile f;
    int line_counter = 0;
    int air_count = 0;
    double rlat, rlon, glat, glon;
    QString name, rwyt;
    int i, rwycnt;
    QString row_code;
    int line_id;
    QStringList parts;
    QString part, chr;
    int pcnt;
    int comm_count = 0;
    if (!pal) {
        pli->msg = "ERROR: AIRPORTLIST * NOT SET or ALLOCATED!";
        return;
    }
    if (!f.exists(zf)) {
        pli->msg = "ERROR: Failed to find ["+zf+"! NO AIRPORT FILE DATA!";
        return;
    }
    tm.start();
    gzf = fgx_gzOpen(zf);
    if (!gzf) {
        pli->msg = "ERROR: Failed to open ["+zf+"]";
        return;
    }
    pli->msg = "Processing file ["+zf+"]";

    QString txwy = "xxx";

    //* ignore first line
    fgx_gzReadline(gzf); // = 'I' undrstand indicates IBM (i.e. DOS line endings)
    line_counter++;
    QString credits = fgx_gzReadline(gzf);  // big 'credit' line
    line_counter++;

    int version = 999;
    if (credits.startsWith("810 Version") ) {
        version = 810;
    } else if (credits.startsWith("850 Version")) {
        version = 850;
    } else if (credits.startsWith("1000 Version")) {
        version = 1000;
    } else {
        fgx_gzClose(gzf);
        pli->msg = "ERROR: Unknown version ["+zf+"]";
        debug_stop(version);
        return;
    }

    rwycnt = 0;
    glat = 0;
    glon = 0;
    bool got_airport = false;
    PAD_AIRPORT pap = new AD_AIRPORT;
    INITPAP(pap);
    int tot_runways = 0;
    while ( ! fgx_gzEof(gzf)) {
        if (pli->work.abort)
            break;
        line_counter++;
        QString line = fgx_gzReadline(gzf);
        if (!(line.size() == 0) && isspace(line.at(0).digitValue())) {
            msg.sprintf("%d",line_counter);
            outLog("CHECK ME! Line commencing with space "+msg+" ["+line+"]");
            debug_stop(line_counter);
        }
        //if (!line.at(0).isDigit()) { /* maybe a warning */ }
        line = line.trimmed();
        if (line.size() == 0)
            continue;
        row_code = line.section(' ',0, 0);
        line_id = row_code.toInt();
        parts = line.split(" ", QString::SkipEmptyParts);
        pcnt = parts.count();
        if (line_id == 99) {
            break;  // end of file
        }
        if ((line_id == 1)||(line_id == 16)||(line_id == 17)) {
            // got airport
            // 1. check for previous airport
            if (got_airport) {
                // time to make an airport entry
                if (rwycnt) {
                    pap->clat = glat / rwycnt;
                    pap->clon = glon / rwycnt;
                } else {
                    msg.sprintf("%d: ",line_counter);
                    msg.append(pap->icao);
                    outLog("CHECK ME! at line "+msg+" previous airport with NO runways!");
                    debug_stop(line_counter);
                }
                pal->push_back(pap);
                pap = new AD_AIRPORT;
                INITPAP(pap);
            }
            got_airport = false;
            if (pcnt >= 6) {
                // get airport
                // http://data.x-plane.com/file_specs/Apt715.htm
                // 0 = airport code
                // 1 = elevation
                // 2 = has tower
                // 3 = not approp
                // 4 = code
                // 5+ name
                pap->elev = parts.at(1).toDouble();
                pap->icao = parts.at(4);
                name = parts.at(5);
                if (fix_name && (name.length() > 2)) {
                    name = name.toLower();
                    chr = name.at(0);
                    chr = chr.toUpper();
                    name.replace(0,1,chr);
                }
                for (i = 6; i < pcnt; i++) {
                    part = parts.at(i);
                    if (fix_name) {
                        part = part.toLower();
                        chr = part.at(0);
                        chr = chr.toUpper();
                        part.replace(0,1,chr);
                    }
                    name.append(" ");
                    name.append(part);
                }
                pap->name = name;
                got_airport = true;
            } else {
                msg.sprintf("%d",line_counter);
                outLog("CHECK ME! Unknown airport line "+msg+" ["+line+"]");
                debug_stop(line_counter);
            }

            rwycnt = 0; // restart runway counter
            glat = 0;
            glon = 0;
        } else if (line_id == 10) {
            // Runway v810
            if (pcnt >= 8) {
                //  0   1          2          3   4       5    6         7           8   9      10 11 12 13   14 15
                //# 10  36.962213  127.031071 14x 131.52  8208 1595.0620 0000.0000   150 321321  1 0  3  0.25 0  0300.0300
                //# 10  36.969145  127.020106 xxx 221.51   329 0.0       0.0          75 161161  1 0  0  0.25 0
                rlat = parts.at(1).toDouble();
                rlon = parts.at(2).toDouble();
                rwyt = parts.at(3);     //# text 'xxx'=taxiway, 'H1x'=heleport, else a runway
                if (rwyt != txwy) {
                    // runway
                    glat += rlat;
                    glon += rlon;
                    rwycnt++;
                    AD_RUNWAY rw;
                    rw.lat = rlat;
                    rw.lon = rlon;
                    // trim trailing 'x'
                    int ind = rwyt.indexOf(QChar('x'));
                    while (ind == (rwyt.length()-1)) {
                        rwyt.chop(1);
                        ind = rwyt.indexOf(QChar('x'));
                    }
                    rw.id = rwyt;
                    rw.hdgt = parts.at(4).toDouble();
                    rw.lenft = parts.at(5).toDouble();
                    if (pli->work.abort)
                        break;
                    pap->run_list.push_back(rw);
                    tot_runways++;
                } else if (add_taxiways) {
                    AD_RUNWAY tw;
                    tw.lat = rlat;
                    tw.lon = rlon;
                    tw.id = rwyt;
                    tw.hdgt = parts.at(4).toDouble();
                    tw.lenft = parts.at(5).toDouble();
                    if (pli->work.abort)
                        break;
                    pap->tax_list.push_back(tw);
                }
            } else {
                msg.sprintf("%d cnt=%d:",line_counter,pcnt);
                outLog("CHECK ME! Unknown runway line "+msg+" ["+line+"]");
                debug_stop(line_counter);
            }
        } else if (line_id == 18) {
            // beacon (ignored)
        } else if (line_id == 19) {
            // windsock location
            if (pcnt >= 4) {
                AD_WINDSOCK ws;
                ws.lat = parts.at(1).toDouble();
                ws.lon = parts.at(2).toDouble();
                ws.elev = parts.at(3).toDouble();
                if (pli->work.abort)
                    break;
                pap->sock_list.push_back(ws);
            } else {
                msg.sprintf("%d",line_counter);
                outLog("CHECK ME! Unknown windsock line "+msg+" ["+line+"]");
                debug_stop(line_counter);
            }
        } else if (line_id == 14) {
            // control tower
            // *  0 = 14 	Identifies this as a data line for a tower viewpoint (code 14).Only a single tower viewpoint is permitted.
            if (pcnt > 5) {
                pap->got_tower = true;
                pap->tlat = parts.at(1).toDouble(); //*  1 = 35.047005 	Latitude (in decimal degrees) of the viewpoint.
                pap->tlon = parts.at(2).toDouble(); //*  2 = -106.608162 	Longitude (in decimal degrees) of the viewpoint.
                pap->televft = parts.at(3).toDouble(); // *  3 = 100 	Height (in feet) above ground level of viewpoint.
                //*  4 = 1 	Flag to indicate if a control tower object should be drawn at this location in X-Plane.  0=no tower, 1=draw tower.
                //*  5 - tower viewpoint 	Name of this viewpoint
            } else {
                msg.sprintf("%d",line_counter);
                outLog("CHECK ME! Unknown tower line "+msg+" ["+line+"]");
                debug_stop(line_counter);
            }
        } else if (line_id == 20) {
            // Taxiway sign
        } else if (line_id == 21) {
            // lighting object
        } else if (line_id == 15) {
            // custom startup
        } else if ((line_id >= 50)&&(line_id <= 56)) {
            // comm information
            if (pcnt > 1) {
                AD_COMMS comm;
                //*  0 = 53 	Identifies this as an airport ATC frequency line.  Codes in the 50 - 59 range are used to identity different ATC types.
                comm.type = line_id;
                //*  1 = 12190 	Airport ATC frequency, in Megahertz multiplied by 100 (ie. 121.90 MHz in this example).
                comm.freq = parts.at(1).toDouble() / 100.0;
                //*  2 = GND 	Name of the ATC frequency.  This is often an abbreviation (such as GND for "Ground").
                if (pcnt > 2)
                    comm.name = parts.at(2);
                else {
                    comm.name = "";
                    // msg.sprintf("%d cnt=%d: ",line_counter,pcnt);
                    // outLog("CHECK ME! COMM line without name "+msg+" ["+line+"]");
                    // debug_stop(line_counter);
                }
                if (pli->work.abort)
                    break;
                pap->comm_list.push_back(comm);
                comm_count++;
            } else {
                msg.sprintf("%d",line_counter);
                outLog("CHECK ME! Unknown COMM line "+msg+" ["+line+"]");
                debug_stop(line_counter);
            }
        } else {
            if ((version == 850)||(version == 1000)) {
                double az1, az2, s, lat1, lon1, lat2, lon2, feet;
                if (line_id == 100) {
                    // * 100  Runway
                    // 0    1     2 3 4    5 6 7 8  9           10            11   12   13 14 15 16 17 18          19            20   21   22 23 24 25
                    // 100  91.14 3 0 0.25 0 0 0 18 40.47295928 -096.98665180 0.00 0.00 0  0  0  0  36 40.47996200 -096.98543200 0.00 0.00 0  0  0  0
                    if (pcnt < 26) {
                        msg.sprintf("%d cnt=%d",line_counter,pcnt);
                        outLog("CHECK ME! Unknown 850 runway line "+msg+" ["+line+"]");
                        debug_stop(line_counter);
                    } else {
                        // ok 850 runway
                        lat1 = parts.at(9).toDouble();
                        lon1 = parts.at(10).toDouble();
                        lat2 = parts.at(18).toDouble();
                        lon2 = parts.at(19).toDouble();
                        if (!LATLONOK(lat1,lon1) || !LATLONOK(lat2,lon2)) {
                            msg.sprintf("%d cnt=%d ",line_counter,pcnt);
                            msg.append( (skip_outofrange ? "skipped" : "fixed") );
                            outLog("CHECK ME! lat/lon OOR 850 runway line "+msg+" ["+line+"]");
                            debug_stop(line_counter);
                            if (skip_outofrange)
                                continue;
                            // OR push into world
                            if (lat1 < -90.0)
                                lat1 = -90.0;
                            else if (lat1 > 90.0)
                                lat1 = 90.0;
                            if (lon1 < -180.0)
                                lon1 = -180.0;
                            else if (lon1 > 180.0)
                                lon1 = 180.0;
                            if (lat2 < -90.0)
                                lat2 = -90.0;
                            else if (lat2 > 90.0)
                                lat2 = 90.0;
                            if (lon2 < -180.0)
                                lon2 = -180.0;
                            else if (lon2 > 180.0)
                                lon2 = 180.0;
                        }
                        sg_geo_inverse_wgs_84( lat1, lon1, lat2, lon2, &az1, &az2, &s );
                        feet = SG_METER_TO_FEET * s;    // convert to FEET
                        // and get runway CENTER
                        sg_geo_direct_wgs_84 ( lat1, lon1, az1, feet / 2.0, &rlat, &rlon, &az2 );
                        glat += rlat;
                        glon += rlon;
                        rwycnt++;
                        AD_RUNWAY rw;
                        rw.lat = rlat;
                        rw.lon = rlon;
                        rw.id = parts.at(8);
                        rw.hdgt = az1;
                        rw.lenft = feet;
                        if (pli->work.abort)
                            break;
                        pap->run_list.push_back(rw);
                        tot_runways++;
                    }
                } else if (line_id == 101) {
                    // * 101  Water runway
                    // 0   1  Width
                    //           2 perimeter buoys
                    //             3  Runway number
                    //                4 Lat
                    //                            5 Lon
                    //       Number, Lat, lon of other end      6  7           8
                    // 101 22.86 0 07 29.12988952 -089.39561501 25 29.13389936 -089.38060001
                    if (pcnt < 9) {
                        msg.sprintf("%d cnt=%d",line_counter,pcnt);
                        outLog("CHECK ME! Unknown 850 waterway line "+msg+" ["+line+"]");
                        debug_stop(line_counter);
                    } else {
                        lat1 = parts.at(4).toDouble();
                        lon1 = parts.at(5).toDouble();
                        lat2 = parts.at(7).toDouble();
                        lon2 = parts.at(8).toDouble();
                        if (!LATLONOK(lat1,lon1) || !LATLONOK(lat2,lon2)) {
                            msg.sprintf("%d cnt=%d ",line_counter,pcnt);
                            msg.append( (skip_outofrange ? "skipped" : "fixed") );
                            outLog("CHECK ME! lat/lon OOR 850 waterway line "+msg+" ["+line+"]");
                            debug_stop(line_counter);
                            if (skip_outofrange)
                                continue;
                            // OR push into world
                            if (lat1 < -90.0)
                                lat1 = -90.0;
                            else if (lat1 > 90.0)
                                lat1 = 90.0;
                            if (lon1 < -180.0)
                                lon1 = -180.0;
                            else if (lon1 > 180.0)
                                lon1 = 180.0;
                            if (lat2 < -90.0)
                                lat2 = -90.0;
                            else if (lat2 > 90.0)
                                lat2 = 90.0;
                            if (lon2 < -180.0)
                                lon2 = -180.0;
                            else if (lon2 > 180.0)
                                lon2 = 180.0;
                        }
                        sg_geo_inverse_wgs_84( lat1, lon1, lat2, lon2, &az1, &az2, &s );
                        feet = SG_METER_TO_FEET * s;    // convert to FEET
                        // and get RUNWAY CENTER
                        sg_geo_direct_wgs_84 ( lat1, lon1, az1, feet / 2.0, &rlat, &rlon, &az2 );
                        glat += rlat;
                        glon += rlon;
                        rwycnt++;
                        AD_RUNWAY rw;
                        rw.lat = rlat;
                        rw.lon = rlon;
                        rw.id = parts.at(3);
                        rw.hdgt = az1;
                        rw.lenft = feet;
                        if (pli->work.abort)
                            break;
                        pap->run_list.push_back(rw);
                        tot_runways++;
                    }
                } else if (line_id == 102) {
                    // * 102  Helipad
                    // 0   1  2           3            4    5     6     7  8 9 10   11
                    // 102 H3 52.48071507 013.39937648 2.64 13.11 13.11 1  0 0 0.00 0
                    if (pcnt < 12) {
                        msg.sprintf("%d cnt=%d",line_counter,pcnt);
                        outLog("CHECK ME! Unknown 850 helipad line "+msg+" ["+line+"]");
                        debug_stop(line_counter);
                    } else {
                        rlat = parts.at(2).toDouble();
                        rlon = parts.at(3).toDouble();
                        if (!LATLONOK(rlat,rlon)) {
                            msg.sprintf("%d cnt=%d ",line_counter,pcnt);
                            msg.append( (skip_outofrange ? "skipped" : "fixed") );
                            outLog("CHECK ME! lat/lon OOR 850 helipad line "+msg+" ["+line+"]");
                            debug_stop(line_counter);
                            if (skip_outofrange)
                                continue;
                            // OR push into world
                            if (rlat < -90.0)
                                rlat = -90.0;
                            else if (rlat > 90.0)
                                rlat = 90.0;
                            if (rlon < -180.0)
                                rlon = -180.0;
                            else if (rlon > 180.0)
                                rlon = 180.0;
                        }
                        rwyt = parts.at(1);     //# 'H1'=heleport
                        // runway
                        glat += rlat;
                        glon += rlon;
                        rwycnt++;
                        AD_RUNWAY rw;
                        rw.lat = rlat;
                        rw.lon = rlon;
                        rw.id = rwyt;
                        rw.hdgt = parts.at(4).toDouble();
                        rw.lenft = parts.at(5).toDouble();
                        if (pli->work.abort)
                            break;
                        pap->run_list.push_back(rw);
                        tot_runways++;
                    }
                } else if (line_id == 110) {
                    // * 110  Pavement (taxiway or ramp) header   Must form a closed loop
                } else if (line_id == 120) {
                    // * 120  Linear feature (painted line or light string) Can form closed loop
                } else if (line_id == 130) {
                    // * 130  Airport boundary header Must form a closed loop
                } else if (line_id == 111) {
                    // * 111  Node All nodes can also include a style (line or lights)
                } else if (line_id == 112) {
                    // * 112  Node with Bezier control point Bezier control points define smooth curves
                } else if (line_id == 113) {
                    // * 113  Node with implicit close of loop Implied join to first node in chain
                } else if (line_id == 114) {
                    // * 114  Node with Bezier control point, with implicit close of loop Implied join to
                    // *      first node in chain
                } else if (line_id == 115) {
                    // * 115  Node terminating a string (no close loop) No styles used
                } else if (line_id == 116) {
                    // * 116  Node with Bezier control point, terminating a string (no close loop) No
                    // *      styles used
                } else if (line_id == 20) {
                    // *  20  Taxiway sign (inc. dist-remain) Zero, one or many for each airport
                } else if (line_id == 21) {
                    // *  21  Lighting (VASI, PAPI, Wig-Wag, etc.) Zero or many for each airport
                } else if (line_id == 120) {
                    // # 120 hold lines W A13
                } else if (line_id == 130) {
                    // # 130 Airport Boundary
                } else if (line_id == 1000) {
                    // # 1000 Northerly flow
                } else if (line_id == 1001) {
                    // # 1001 KGRB 270 020 999
                } else if (line_id == 1002) {
                    // # 1002 KGRB 0
                } else if (line_id == 1003) {
                    // # 1003 KGRB 0
                } else if (line_id == 1004) {
                    // # 1004 0000 2400
                } else if (line_id == 1100) {
                    // # 1100 36 12654 all heavy|jets|turboprops|props 000360 000360 Northerly
                } else if (line_id == 1101) {
                    // # 1101 36 left
                } else if (line_id == 1200) {
                    // # ????
                } else if (line_id == 1201) {
                    // # 1201 42.75457409 -073.80880021 both 2110 _start
                } else if (line_id == 1202) {
                    // # 1202 2110 2112 twoway taxiway
                } else if (line_id == 1204) {
                    // # 1204 arrival 01,19
                } else if (line_id == 1300) {
                    // # 1300 30.32875704 -009.41140596 323.85 misc jets|props Ramp
                } else {
                    msg.sprintf("%d cnt=%d",line_counter,pcnt);
                    outLog("CHECK ME! Unknown 850 line "+msg+" ["+line+"]");
                    debug_stop(line_counter);
                }
            } else {
                msg.sprintf("%d cnt=%d",line_counter,pcnt);
                outLog("CHECK ME! Unknown 810 line "+msg+" ["+line+"]");
                debug_stop(line_counter);
            }
        }
    }
    // get any final entry
    if (got_airport && !pli->work.abort) {
        pal->push_back(pap);
    } else
        delete pap;

    // done full file
    fgx_gzClose(gzf);   // close it
    pli->loadTime_ms = tm.elapsed();
    if (pli->work.abort) {
        msg = "Aborted after "+getElapTimeStg(pli->loadTime_ms);
    } else {
        msg.sprintf("Found %d COMM items in file.", comm_count);
        outLog(msg);
        air_count = pal->count();
        msg.sprintf("Done %d lines, found %d airport, %d runways",line_counter,air_count,tot_runways);
        msg.append(", in "+getElapTimeStg(pli->loadTime_ms));
        pli->result = true;    // RETURN SUCCESS
    }
    pli->msg = msg; // final message
    outLog(msg);
}

// ================================

QString loadAptDat::getAiportStg(PAD_AIRPORT pad, int flag)
{
    int cnt, i;
    QString reply(pad->icao);
    QString msg;
    // get basic information
    msg.sprintf(" lat,lon %f,%f",pad->clat,pad->clon);
    reply.append(msg);
    msg.sprintf(" alt. %4d ft.", (int)pad->elev);
    reply.append(msg);
    reply.append(" "+pad->name);

    // add runway information
    if (flag & 1) {
        cnt = pad->run_list.count();
        msg.sprintf("\nRunways %d: ",cnt);
        reply.append(", "+msg);
        for (i = 0; i < cnt; i++) {
            AD_RUNWAY rw = pad->run_list.at(i);
            reply.append(" "+rw.id);
            msg.sprintf(" %d ft", (int)rw.lenft);
            reply.append(msg);
            msg.sprintf(" %3.1fT",rw.hdgt);
            reply.append(msg);
        }
    }

    // add radio info
    if (flag & 2) {
        cnt = pad->comm_list.count();
        if (cnt)
            reply.append("\nComms: ");
        for (i = 0; i < cnt; i++) {
            AD_COMMS comms = pad->comm_list.at(i);
            msg.sprintf(" %3.2f ", comms.freq);
            reply.append(comms.name+msg);
        }
    }

    return reply;
}

PAD_AIRPORT loadAptDat::findAirportByICAO(QString icao)
{
    AIRPORTLIST *pal = getAirListPtr();
    int i, max = pal->count();
    for (i = 0; i < max; i++) {
        PAD_AIRPORT pad = pal->at(i);
        if (pad->icao == icao)
            return pad;
    }
    return 0;
}

QString loadAptDat::findNameByICAO(QString icao, int flag)
{
    QString name;
    Q_UNUSED(flag);
    PAD_AIRPORT pad = findAirportByICAO(icao);
    if (pad) {
        name = pad->name;
        // NOTE: FIX NAME has beeen moved to work on the THREAD
        // ====================================================
    }
    return name;
}

bool cmpICAO( const PAD_AIRPORT p1, const PAD_AIRPORT p2)
{
    return (p1->icao < p2->icao);
}
void sortByICAO(AIRPORTLIST *pal)
{
    qSort(pal->begin(), pal->end(), cmpICAO);
}

bool cmpDistance( const PAD_AIRPORT p1, const PAD_AIRPORT p2)
{
    return p1->distance_km < p2->distance_km;
}


AIRPORTLIST *loadAptDat::getNearestAiportList(PAD_AIRPORT pad, int flag)
{
    if (!pad)
        return 0;
    if (findAirportByICAO(pad->icao) != pad)
        return 0;

    AIRPORTLIST *pal = getAirListPtr();
    int i, max = pal->count();
    for (i = 0; i < max; i++) {
        PAD_AIRPORT pad2 = pal->at(i);
        pad2->distance_km = util_DistEst_km(pad->clat,pad->clon,pad2->clat,pad2->clon);
        pad2->rank = 0; // clear rank
    }
    qSort(pal->begin(), pal->end(), cmpDistance);

    Q_UNUSED(flag);
    return pal;
}

bool loadAptDat::isThreadInFunction()
{
    if (workthread && workthread->in_function)
        return true;
    return false;
}


// eof - loadAptDat.cpp
