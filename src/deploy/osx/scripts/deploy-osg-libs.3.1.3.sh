#!/bin/bash

FILES="libosgWidget.92.dylib libosgVolume.92.dylib libosgViewer.92.dylib libosgUtil.92.dylib libosgText.92.dylib libosgTerrain.92.dylib libosgSim.92.dylib libOpenThreads.12.dylib libosg.92.dylib libosgAnimation.92.dylib libosgDB.92.dylib libosgFX.92.dylib libosgGA.92.dylib libosgManipulator.92.dylib libosgParticle.92.dylib libosgPresentation.92.dylib libosgQt.92.dylib libosgShadow.92.dylib"

for f in $FILES

	do
		install_name_tool -change libosgDB.92.dylib @executable_path/../Frameworks/libosgDB.92.dylib $f
		install_name_tool -change libosgSim.92.dylib @executable_path/../Frameworks/libosgSim.92.dylib $f
		install_name_tool -change libosgUtil.92.dylib @executable_path/../Frameworks/libosgUtil.92.dylib $f
		install_name_tool -change libosg.92.dylib @executable_path/../Frameworks/libosg.92.dylib $f
		install_name_tool -change libOpenThreads.12.dylib @executable_path/../Frameworks/libOpenThreads.12.dylib $f
		install_name_tool -change libosgText.92.dylib @executable_path/../Frameworks/libosgText.92.dylib $f
		install_name_tool -change libosgFX.92.dylib @executable_path/../Frameworks/libosgFX.92.dylib $f
		install_name_tool -change libosgParticle.92.dylib @executable_path/../Frameworks/libosgParticle.92.dylib $f
		install_name_tool -change libosgGA.92.dylib @executable_path/../Frameworks/libosgGA.92.dylib $f
		install_name_tool -change libosgQt.92.dylib @executable_path/../Frameworks/libosgQt.92.dylib $f
		install_name_tool -change libosgViewer.92.dylib @executable_path/../Frameworks/libosgViewer.92.dylib $f
		install_name_tool -change libosgSim.92.dylib @executable_path/../Frameworks/libosgSim.92.dylib $f
		install_name_tool -change libosgManipulator.92.dylib @executable_path/../Frameworks/libosgManipulator.92.dylib $f
		install_name_tool -change libosgPresentation.92.dylib @executable_path/../Frameworks/libosgPresentation.92.dylib $f
		install_name_tool -change libosgQt.92.dylib @executable_path/../Frameworks/libosgQt.92.dylib $f
		install_name_tool -change libosgShadow.92.dylib @executable_path/../Frameworks/libosgShadow.92.dylib $f
		install_name_tool -change libosgTerrain.92.dylib @executable_path/../Frameworks/libosgTerrain.92.dylib $f
		install_name_tool -change libosgVolume.92.dylib @executable_path/../Frameworks/libosgVolume.92.dylib $f
		install_name_tool -change libosgWidget.92.dylib @executable_path/../Frameworks/libosgWidget.92.dylib $f
	
		# Show result in log
		otool -L $f
done