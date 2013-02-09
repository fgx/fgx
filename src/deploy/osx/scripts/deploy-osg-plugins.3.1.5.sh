#!/bin/bash

FILES = *.so

for f in *

do
    install_name_tool -change libosg.97.dylib @executable_path/../Frameworks/libosg.97.dylib $f
    install_name_tool -change libosgSim.97.dylib @executable_path/../Frameworks/libosgSim.97.dylib $f
    install_name_tool -change libosgAnimation.97.dylib @executable_path/../Frameworks/libosgAnimation.97.dylib $f
    install_name_tool -change libosgTerrain.97.dylib @executable_path/../Frameworks/libosgTerrain.97.dylib $f
    install_name_tool -change libosgDB.97.dylib	@executable_path/../Frameworks/libosgDB.97.dylib $f
    install_name_tool -change libosgText.97.dylib @executable_path/../Frameworks/libosgText.97.dylib $f
    install_name_tool -change libosgFX.97.dylib	@executable_path/../Frameworks/libosgFX.97.dylib $f
    install_name_tool -change libosgUtil.97.dylib @executable_path/../Frameworks/libosgUtil.97.dylib $f
    install_name_tool -change libosgGA.97.dylib	@executable_path/../Frameworks/libosgGA.97.dylib $f	
    install_name_tool -change libosgViewer.97.dylib @executable_path/../Frameworks/libosgViewer.97.dylib $f
    install_name_tool -change libosgManipulator.97.dylib @executable_path/../Frameworks/libosgManipulator.97.dylib $f
    install_name_tool -change libosgVolume.97.dylib @executable_path/../Frameworks/libosgVolume.97.dylib $f
    install_name_tool -change libosgParticle.97.dylib @executable_path/../Frameworks/libosgParticle.97.dylib $f
    install_name_tool -change libosgWidget.97.dylib @executable_path/../Frameworks/libosgWidget.97.dylib $f
    install_name_tool -change libosgPresentation.97.dylib @executable_path/../Frameworks/libosgPresentation.97.dylib $f
    install_name_tool -change libosgQt.97.dylib @executable_path/../Frameworks/libosgQt.97.dylib $f
    install_name_tool -change libOpenThreads.12.dylib @executable_path/../Frameworks/libOpenThreads.12.dylib $f
    install_name_tool -change libosgShadow.97.dylib @executable_path/../Frameworks/libosgShadow.97.dylib $f
    
    otool -L $f
done



