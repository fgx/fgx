README.minizip.txt - 20150318

This source was copied from ZLIB 1.2.8 source, http://www.zlib.net/, version dated 
April 28, 2013. It is under the same 'open' license as zlib - http://www.zlib.net/zlib_license.html

This provides the glue to deal with ZIP files, which can contains a list of 
files, all contained in the one zip file.

In this case the sources, except for miniunz.c and minizip.c are combined into a minizip 
library.

miniunz.c and minizip.c are 'mini' command line apps to unzip a zip, either just 
listing the files, or extracting them and writing them out to a directory, or 
compress that data of a list of files into a zip file.

Naturally all the decompression and compression of data depends on zlib itself.


;eof
