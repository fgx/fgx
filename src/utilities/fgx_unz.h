/*\
 * fgx_unz.h
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#ifndef _FGX_UNZ_H_
#define _FGX_UNZ_H_

// unzip errors
#define unze_none           0
#define unze_no_zip_file    1
#define unze_no_out_dir     2
#define unze_no_get_info    3
#define unze_no_file_info   4
#define unze_no_get_next    5
#define unze_no_chdir       6
#define unze_no_memory      7
#define unze_file_in_way    8
#define unze_no_create_dir  9
#define unze_no_open_curpwd 10
#define unze_no_create_file 11
#define unze_failed_read    12
#define unze_failed_write   13
#define unze_close_curr     14

// unzip flags
#define uzflg_listonly  0x00000001
#define uzflg_nopath    0x00000002
#define uzflg_noover    0x00000004



extern int fileUnzip( QString &file, QString &dir, uint flag = 0, const char *password = 0 );
extern std::string getUzErrorString();

#endif // #ifndef _FGX_UNZ_H_
// eof - fgx_unz.h
