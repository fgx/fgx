--- Libs, Frameworks ---

# ALUT framework
James Turner (FlightGear) created a ALUT.framework that can be used in your frameworks folder in /Librarys, or as a framework in the FGx bundle. ALUT is used by FlightGear and I need it for the FGx OSX bundle.
http://connect.creativelabs.com/openal/Documentation/The%20OpenAL%20Utility%20Toolkit.htm 

# libjpeg.8.dylib
Need that for the bundle in case OSG is configured to use cocoa/imageio. It’s locally derived by macports, and libnametooled to be shipped with FGx.
http://www.macports.org/ports.php?by=library&substr=jpeg
http://www.ijg.org/

--- Patches ---

# osg-cmakelist.patch
created to patch cmakelist.txt, which works only for compiling with a
cmake version I don't want to install at the moment. Reading the OSG
forums it seems also that changed cmakelist from 3.1.0 to 3.1.2 will 
break other things. After patch applied cmakelist is set to use 
macosx10.5.sdk. In case you wish to use another sdk just lookout for
this "hardcoded" cmakelist change and change to the value you need.
I'm just happy getting OSG compiled at the moment, this patch should be
removed asap.

# osg-cocoa-3.1.2.patch
This is a changed patch (origin by Tat N. for macflightgear.org) to make
some flightgear specific changes, i.e. to give back ctrl-c to see hotspots
for osx cocoa users. And other stuff.


