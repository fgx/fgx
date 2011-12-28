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
// #define USE_ALTERNATE_GETFILE
#undef USE_ALTERNATE_GETFILE

#define DEF_TO_MS   10000   // 20111227 - was 5000

#endif // APP_CONFIG_H
// app_config.h
