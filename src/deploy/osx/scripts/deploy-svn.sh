#!/bin/bash

FILES="libsvn_client-1.0.dylib libsvn_subr-1.0.dylib libsvn_ra-1.0.dylib libsvn_wc-1.0.dylib libsvn_delta-1.0.dylib libsvn_diff-1.0.dylib libsvn_ra_local-1.0.dylib libsvn_repos-1.0.dylib libsvn_fs-1.0.dylib libsvn_fs_fs-1.0.dylib libsvn_fs_util-1.0.dylib libsvn_ra_svn-1.0.dylib libsvn_ra_neon-1.0.dylib libneon.27.dylib"

for f in $FILES

do
	
	# link svn
	install_name_tool -change /usr/lib/libsvn_client-1.0.dylib @executable_path/../Frameworks/libsvn_client-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_subr-1.0.dylib @executable_path/../Frameworks/libsvn_subr-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_ra-1.0.dylib @executable_path/../Frameworks/libsvn_ra-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_wc-1.0.dylib @executable_path/../Frameworks/libsvn_wc-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_delta-1.0.dylib @executable_path/../Frameworks/libsvn_delta-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_diff-1.0.dylib @executable_path/../Frameworks/libsvn_diff-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_ra_local-1.0.dylib @executable_path/../Frameworks/libsvn_ra_local-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_repos-1.0.dylib @executable_path/../Frameworks/libsvn_repos-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_fs-1.0.dylib @executable_path/../Frameworks/libsvn_fs-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_fs_fs-1.0.dylib @executable_path/../Frameworks/libsvn_fs_fs-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_fs_util-1.0.dylib @executable_path/../Frameworks/libsvn_fs_util-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_ra_svn-1.0.dylib @executable_path/../Frameworks/libsvn_ra_svn-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_ra_neon-1.0.dylib @executable_path/../Frameworks/libsvn_ra_neon-1.0.dylib $f
	
	install_name_tool -change /usr/lib/libneon.27.dylib @executable_path/../Frameworks/libneon.27.dylib $f
	
	# Show result in log
	otool -L $f
done
