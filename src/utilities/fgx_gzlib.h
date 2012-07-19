/* ********************************************************
   *  fgx_gzlib.h
   *  for fgx project
   *
   *  Created by Geoff R. Mclane, Paris
   *  (c) 2011 GPL2 (or later)
   *
   ******************************************************** */
#ifndef FGX_GZLIB_H
#define FGX_GZLIB_H

#include <QString>

typedef void * fgx_gzHandle; // opaque HANDLE to gz file

extern fgx_gzHandle fgx_gzOpen(QString file);
extern QString fgx_gzReadline(fgx_gzHandle hand);
extern int fgx_gzEof(fgx_gzHandle hand);
extern int fgx_gzClose(fgx_gzHandle hand);


#endif // FGX_GZLIB_H
// eof - fgx_gzlib.h
