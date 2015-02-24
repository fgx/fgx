/*
 *  app_config.h
 *  FGx
 *
 *  Created by Geoff R. McLane, France
 *  (c) 2011 --- GPL2
 *
 * GLOBAL application configuration
 * Must ONLY contain #defines
 */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include <QtGui>    // for Q_OS_???? macros, if needed

/* define to use 'alternate' functions for get-file and get-directory */
// #define USE_ALTERNATE_GETFILE
#undef USE_ALTERNATE_GETFILE

#define DEF_TO_MS   10000   // 20111227 - was 5000

// 20120127 - NEW features
// enable thread load of apt.dat.gz
#define ENABLE_APT_DAT_LOAD
// use current USER scenery paths
#define ENABLE_NEW_AIRPORT_PATH

// Thread - User types
#define tht_loadAptDat  1

    enum OS_TYPE {
        OS_BSD = 1,
        OS_MAC = 2,
        OS_LINUX = 3,
        OS_WINDOWS = 4,
        OS_UNKNOWN = 5,
    };

#endif // APP_CONFIG_H
// app_config.h


