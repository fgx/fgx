/* ********************************************************
   *  loadAptDat
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Dec 2011 GPL2 (or later)
   *
   *  Load the flightgear apt.dat.gz file
   *
   ******************************************************** */
#ifndef LOADAPTDAT_H
#define LOADAPTDAT_H
#include "app_config.h"
#include <QWidget>
#include <QString>
#include <QTime>
#include <QFile>
#include "utilities/fgx_gzlib.h"
#include "utilities/workThread.h"

typedef struct tagAD_WINDSOCK {
    double lat,lon,elev;
}AD_WINDSOCK;
typedef QList<AD_WINDSOCK> SOCKLIST;

typedef struct tagAD_COMMS {
    int type;       // 50-56 like 53 Identifies this as an airport ATC frequency line.
    double freq;    // 12190 / 100 - Airport ATC frequency, in Megahertz (ie. 121.90 MHz in this example).
    QString name;   // like GND - Name of the ATC frequency.  This is often an abbreviation (such as GND for "Ground").
}AD_COMMS;
typedef QList<AD_COMMS> COMMLIST;

typedef struct tagAD_RUNWAY {
    double lat,lon, hdgt, lenft;
    QString id;
}AD_RUNWAY;
typedef QList<AD_RUNWAY> RUNWAYLIST;

typedef struct tagAD_AIRPORT {
    QString icao;
    QString name;
    double elev;
    double clat,clon;
    double tlat,tlon,televft;
    bool got_tower;
    SOCKLIST sock_list;
    RUNWAYLIST run_list;
    RUNWAYLIST tax_list;
    COMMLIST comm_list;
    int rank;
    double distance_km;
}AD_AIRPORT, *PAD_AIRPORT;
typedef QList<PAD_AIRPORT> AIRPORTLIST;
typedef AIRPORTLIST * PAIRPORTLIST;
#define INITPAP(a) a->got_tower = false

typedef struct tagLOADITEM {
    WORKITEM work;  // *MUST* be first
    int threadnum;
    QString zf;
    AIRPORTLIST * pAirList; // fill this list
    int loadTime_ms;
    int optionFlag;
    bool result;
    bool in_loading;
    QString msg;
}LOADITEM, *PLOADITEM;

#define lf_AddTax   0x00000001
#define lf_SkipOOR  0x00000002  // SKIP out-of-range values, else FIX
#define lf_FixName  0x00000004  // fix case of name

class loadAptDat : public QObject
{
    Q_OBJECT
public:
    explicit loadAptDat(QObject *par = 0);
    ~loadAptDat();
    QString fgroot;
    bool loadDirect(QString);
    void loadOnThread(QString);
    PAIRPORTLIST getAirListPtr() { return pAirList; }
    PAIRPORTLIST setAirListPtr(PAIRPORTLIST);
    void clear_list(PAIRPORTLIST pal = 0);
    int getAirportCount();
    int getRunwayCount();
    int loadTime_ms;
    int getLoadTime() { return loadTime_ms; }
    PAD_AIRPORT findAirportByICAO(QString);
    QString findNameByICAO(QString icao, int flag = 0);
    QString getAiportStg(PAD_AIRPORT pad, int flag = 0);
    AIRPORTLIST *getNearestAiportList(PAD_AIRPORT pad, int flag = 0);
    bool isThreadInFunction();
    LOADITEM loadItem;  // structure passed to thread

signals:
    void load_done();

public slots:
    void thread_done(int,int);

private:
    PAIRPORTLIST pAirList;
    static void _loadStatic(void *vp);
    workThread *workthread;
};

extern void sortByICAO(AIRPORTLIST *pal);


#endif // LOADAPTDAT_H
// eof - loadAptdat.h
