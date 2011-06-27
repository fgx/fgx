#include "utilities/uncompress.h"


typedef struct {
	quint8 id1;
	quint8 id2;
	quint8 cm;
	quint8 flg;
	float mtime;
	quint8 xfl;
	quint8 os;
} t_gzip_header;

//bit flags
#define gzip_hdr_FTEXT 0
#define gzip_hdr_FHCRC 1
#define gzip_hdr_FEXTRA 2
#define gzip_hdr_FNAME 3
#define gzip_hdr_FCOMMENT 4

void rvln::test()
{
	QFile file ("/home/test/test.cpp.gz");
	if (! file.open (QFile::ReadOnly))
		return;

	t_gzip_header gzip_header;

	QDataStream s_in (&file);

	s_in >> gzip_header.id1;
	s_in >> gzip_header.id2;
	s_in >> gzip_header.cm;
	s_in >> gzip_header.flg;
	s_in >> gzip_header.mtime;
	s_in >> gzip_header.xfl;
	s_in >> gzip_header.os;

	if (gzip_header.flg & (1 << gzip_hdr_FEXTRA))
	{
		quint16 size;
		s_in >> size;
		char *data = new char [size];
		s_in.readRawData (data,size);
		delete data;
	}
	if (gzip_header.flg & (1 << gzip_hdr_FNAME))
	{
		qint8 c;
		QString original_fname;
		do
		{
			s_in >> c;
			original_fname += static_cast<char> (c);
		}
		while (c != 0);
		
		qDebug() << original_fname;
	}
	if (gzip_header.flg & (1 << gzip_hdr_FCOMMENT))
	{
		qint8 c;
		QString comment;
		do
		{
			s_in >> c;
			comment += static_cast<char> (c);
		while (c != 0);
		qDebug() << comment;
	}
	if (gzip_header.flg & (1 << gzip_hdr_FHCRC))
	{
		quint16 crc16;
		s_in >> crc16;
	}
	int len = file.size() - sizeof (t_gzip_header);
	char *compressed_data = new char [len];
	s_in.readRawData (compressed_data, len);

	QByteArray ta;
	len *= 10;

	ta.resize(4);

	ta[0] = (len & 0xff000000) >> 24;
	ta[1] = (len & 0x00ff0000) >> 16;
	ta[2] = (len & 0x0000ff00) >> 8;
	ta[3] = (len & 0x000000ff);

	ta.append (compressed_data);
	ta.chop(8);
	QByteArray aucomp = qUncompress (ta);
	file.close();
}