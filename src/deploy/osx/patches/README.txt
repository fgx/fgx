You will find some patches here for OSG trunk as of May 12th 2012 for OSX.
I'm applying this patches for OSG compilation on a OSX jenkins slave. Maybe
it's also useful for other tasks, but there is NO WARRANTY at all.

osg-cmakelist.patch
created to patch cmakelist.txt, which works only for compiling with a
cmake version I don't want to install at the moment. Reading the OSG
forums it seems also that changed cmakelist from 3.1.0 to 3.1.2 will 
break other things. After patch applied cmakelist is set to use 
macosx10.5.sdk. In case you wish to use another sdk just lookout for
this "hardcoded" cmakelist change and change to the value you need.
I'm just happy getting OSG compiled at the moment, this patch should be
removed asap.

osg-cocoa-3.1.2.patch
This is a changed patch (origin by Tat N. for macflightgear.org) to make
some flightgear specific changes, i.e. to give back ctrl-c to see hotspots
for osx cocoa users. And other stuff.


