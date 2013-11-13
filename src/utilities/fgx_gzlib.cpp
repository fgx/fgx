/* ********************************************************
   *  fgx_gzlib.cpp
   *  for fgx
   *
   *  Created by Geoff R. Mclane, Paris
   *  (c) 2011 GPL2 (or later)
   *
   ******************************************************** */

#include <QString>
#include <QByteArray>
#ifdef USE_SYSTEM_ZLIB
#include <zlib.h>
#else
#include "zlib/fgx_zlib.h"
#endif

typedef void * fgx_gzHandle;

#define MX_GZ_BUFFER 8*1024  // read zlib internal uses 8K buffer, so...
// #define MX_GZ_BUFFER 256    // little buffer - for testing logic only

typedef struct tagGZHANDLE {
    gzFile gzf;
    char gz_buf[MX_GZ_BUFFER+2]; // NOTE: the +2 for zero termination
    int gz_in_buffer;
    int gz_in_offset;

}GZHANDLE, * PGZHANDLE;


fgx_gzHandle fgx_gzOpen(QString file)
{
    QByteArray ba = file.toLocal8Bit();
    const char * cp = ba.data();
    gzFile gzf = gzopen(cp,"rb");
    if (!gzf)
        return 0;
    PGZHANDLE gzh = new GZHANDLE;
    if (!gzh) {
        gzclose(gzf);
        return 0;
    }
    gzh->gzf = gzf;
    gzh->gz_in_buffer = 0;
    gzh->gz_in_offset = 0;
    return gzh;
}

QString fgx_gzReadline(fgx_gzHandle hand)
{
    QString data;
    data = "";
    if (hand) {
        PGZHANDLE pgzh = (PGZHANDLE)hand;
        if (pgzh->gzf && !gzeof(pgzh->gzf)) {
            gzFile file = pgzh->gzf;
            QString blk;
            int i, len;
            char c;
            char * cp = pgzh->gz_buf;
            char * bgn;
            if (!file)
                return data;
            while (!gzeof(file)) {
                if (pgzh->gz_in_offset == 0) {
                    pgzh->gz_in_buffer = gzread(file, cp, MX_GZ_BUFFER);
                }
                if (pgzh->gz_in_buffer) {
                    c = 0;
                    bgn = &cp[pgzh->gz_in_offset];
                    for (i = pgzh->gz_in_offset; i < pgzh->gz_in_buffer; i++) {
                        c = cp[i];
                        if (c == '\n') {
                            break;
                        }
                    }
                    if (c == '\n') {
                        int j = i;
                        while (j >= pgzh->gz_in_offset) {
                            if (cp[j] > ' ')
                                break;  // stop at better than space
                            cp[j] = 0;  // trim tail of data
                            j--;        // backup
                        }
                        blk = bgn;              // get the BEGIN pointer
                        data.append(blk);       // add the data
                        pgzh->gz_in_offset = i + 1;   // update next input
                        if (pgzh->gz_in_offset >= pgzh->gz_in_buffer)
                            pgzh->gz_in_offset = 0;   // deal with wrap
                        break;  // RETURN the accumuated data
                    }
                    cp[i] = 0;   // NOTE: space has been made for this OVERRUN
                    blk = bgn;    // data to QString
                    data.append(blk); // accumulate data
                    pgzh->gz_in_offset = 0;   // restart offset
                } else
                    break;
            }
            len = data.length(); // just for DEBUG
            bgn = &pgzh->gz_buf[pgzh->gz_in_offset];
            i = pgzh->gz_in_offset;
        }
    }
    return data;
}

int fgx_gzEof(fgx_gzHandle hand)
{
    if (!hand)
        return 1;
    PGZHANDLE pgzh = (PGZHANDLE)hand;
    if (!pgzh->gzf)
        return 1;
    return gzeof(pgzh->gzf);
}

int fgx_gzClose(fgx_gzHandle hand)
{
    if (!hand)
        return 1;
    PGZHANDLE pgzh = (PGZHANDLE)hand;
    if (!pgzh->gzf) {
        delete pgzh;
        return 1;
    }
    int ret = gzclose(pgzh->gzf);
    delete pgzh;
    return (ret ? 1 : 0);
}

// eof - fgx_gzlib.cpp
