/*
 *  app_config.h
 *  FGx
 *
 *  Created by Geoff R. McLane, France
 *  © 2011 --- GPL2
 *
 * GLOBAL application configuration
 * Must ONLY contain #defines
 */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include <QtGui>    // for Q_OS_???? macros, if needed

/* define to use 'alternate' functions for get-file and get-directory */
#define USE_ALTERNATE_GETFILE
//#undef USE_ALTERNATE_GETFILE

#define DEF_TO_MS   10000   // 20111227 - was 5000

// prepare to use USER current scenery paths
// #define ENABLE_NEW_AIRPORT_PATH
#undef ENABLE_NEW_AIRPORT_PATH
// enable thread load of apt.dat.gz
#define ENABLE_APT_DAT_LOAD

// Thread - User types
#define tht_loadAptDat  1

#endif // APP_CONFIG_H
// app_config.h
