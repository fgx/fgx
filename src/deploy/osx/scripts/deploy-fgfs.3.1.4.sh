#!/bin/bash

FILES="fgcom fgfs fgjs fgviewer js_demo metar terrasync yasim yasim-proptest"

for f in $FILES

do
	# link OSG
	install_name_tool -change libosgDB.95.dylib @executable_path/../Frameworks/libosgDB.95.dylib $f
	install_name_tool -change libosgSim.95.dylib @executable_path/../Frameworks/libosgSim.95.dylib $f
	install_name_tool -change libosgUtil.95.dylib @executable_path/../Frameworks/libosgUtil.95.dylib $f
	install_name_tool -change libosg.95.dylib @executable_path/../Frameworks/libosg.95.dylib $f
	install_name_tool -change libOpenThreads.12.dylib @executable_path/../Frameworks/libOpenThreads.12.dylib $f
	install_name_tool -change libosgText.95.dylib @executable_path/../Frameworks/libosgText.95.dylib $f
	install_name_tool -change libosgFX.95.dylib @executable_path/../Frameworks/libosgFX.95.dylib $f
	install_name_tool -change libosgParticle.95.dylib @executable_path/../Frameworks/libosgParticle.95.dylib $f
	install_name_tool -change libosgGA.95.dylib @executable_path/../Frameworks/libosgGA.95.dylib $f
	install_name_tool -change libosgQt.95.dylib @executable_path/../Frameworks/libosgQt.95.dylib $f
	install_name_tool -change libosgViewer.95.dylib @executable_path/../Frameworks/libosgViewer.95.dylib $f
	install_name_tool -change libosgSim.95.dylib @executable_path/../Frameworks/libosgSim.95.dylib $f
	install_name_tool -change libosgManipulator.95.dylib @executable_path/../Frameworks/libosgManipulator.95.dylib $f
	install_name_tool -change libosgPresentation.95.dylib @executable_path/../Frameworks/libosgPresentation.95.dylib $f
	install_name_tool -change libosgQt.95.dylib @executable_path/../Frameworks/libosgQt.95.dylib $f
	install_name_tool -change libosgShadow.95.dylib @executable_path/../Frameworks/libosgShadow.95.dylib $f
	install_name_tool -change libosgTerrain.95.dylib @executable_path/../Frameworks/libosgTerrain.95.dylib $f
	install_name_tool -change libosgVolume.95.dylib @executable_path/../Frameworks/libosgVolume.95.dylib $f
	install_name_tool -change libosgWidget.95.dylib @executable_path/../Frameworks/libosgWidget.95.dylib $f
	
	# link ALUT (not needed for FGx >= 2.10.0 anymore, thanks James!) 
	#install_name_tool -change /Users/jmt/Library/Frameworks/ALUT.framework/Versions/A/ALUT @executable_path/../Frameworks//ALUT.framework/Versions/A/ALUT $f
	
	# link libjepeg (imageio/cocoa related)
	install_name_tool -change /opt/local/lib/libjpeg.8.dylib @executable_path/../Frameworks/libjpeg.8.dylib $f
	
	# link svn
	install_name_tool -change /usr/lib/libsvn_client-1.0.dylib @executable_path/../Frameworks/libsvn_client-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_subr-1.0.dylib @executable_path/../Frameworks/libsvn_subr-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_ra-1.0.dylib @executable_path/../Frameworks/libsvn_ra-1.0.dylib $f
	install_name_tool -change /usr/lib/libsvn_wc-1.0.dylib @executable_path/../Frameworks/libsvn_wc-1.0.dylib $f
	
	# link openrti
	install_name_tool -change libRTI-NG.1.dylib @executable_path/../Frameworks/libRTI-NG.1.dylib $f
	install_name_tool -change libFedTime.1.dylib @executable_path/../Frameworks/libFedTime.1.dylib $f
	
	# Show result in log
	otool -L $f
done
