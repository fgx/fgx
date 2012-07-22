#!/bin/bash

FILES="libRTI-NG.1.dylib libFedTime.1.dylib libOpenRTI"

for f in $FILES

do
	# link openrti
	install_name_tool -change libRTI-NG.1.dylib @executable_path/../Frameworks/libRTI-NG.1.dylib $f
	install_name_tool -change libFedTime.1.dylib @executable_path/../Frameworks/libFedTime.1.dylib $f
	install_name_tool -change libOpenRTI.1.dylib @executable_path/../Frameworks/libOpenRTI.1.dylib $f
	
	# Show result in log
	otool -L $f
done
