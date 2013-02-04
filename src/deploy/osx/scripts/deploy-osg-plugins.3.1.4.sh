#!/bin/bash

FILES = *.so

for f in *

do
    install_name_tool -change libosg.95.dylib @executable_path/../Frameworks/libosg.95.dylib $f
    install_name_tool -change libosgSim.95.dylib @executable_path/../Frameworks/libosgSim.95.dylib $f
    install_name_tool -change libosgAnimation.95.dylib @executable_path/../Frameworks/libosgAnimation.95.dylib $f
    install_name_tool -change libosgTerrain.95.dylib @executable_path/../Frameworks/libosgTerrain.95.dylib $f
    install_name_tool -change libosgDB.95.dylib	@executable_path/../Frameworks/libosgDB.95.dylib $f
    install_name_tool -change libosgText.95.dylib @executable_path/../Frameworks/libosgText.95.dylib $f
    install_name_tool -change libosgFX.95.dylib	@executable_path/../Frameworks/libosgFX.95.dylib $f
    install_name_tool -change libosgUtil.95.dylib @executable_path/../Frameworks/libosgUtil.95.dylib $f
    install_name_tool -change libosgGA.95.dylib	@executable_path/../Frameworks/libosgGA.95.dylib $f	
    install_name_tool -change libosgViewer.95.dylib @executable_path/../Frameworks/libosgViewer.95.dylib $f
    install_name_tool -change libosgManipulator.95.dylib @executable_path/../Frameworks/libosgManipulator.95.dylib $f
    install_name_tool -change libosgVolume.95.dylib @executable_path/../Frameworks/libosgVolume.95.dylib $f
    install_name_tool -change libosgParticle.95.dylib @executable_path/../Frameworks/libosgParticle.95.dylib $f
    install_name_tool -change libosgWidget.95.dylib @executable_path/../Frameworks/libosgWidget.95.dylib $f
    install_name_tool -change libosgPresentation.95.dylib @executable_path/../Frameworks/libosgPresentation.95.dylib $f
    install_name_tool -change libosgQt.95.dylib @executable_path/../Frameworks/libosgQt.95.dylib $f
    install_name_tool -change libOpenThreads.12.dylib @executable_path/../Frameworks/libOpenThreads.12.dylib $f
    install_name_tool -change libosgShadow.95.dylib @executable_path/../Frameworks/libosgShadow.95.dylib $f
    
    otool -L $f
done



