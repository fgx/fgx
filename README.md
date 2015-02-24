FGx 
========

**Cross platform launcher for FlightGear**

HomePage: http://fgx/freeflightsim.org 

* FGx is
  * written in c++ using qt toolkit 
  * around spring 2015 expect qt5 (can u help ?)
* Issues, Feature Requests & Bugs: 
  * https://github.com/fgx/fgx/issues
* Code: 
  * https://github.com/fgx/fgx
* API Docs: 
  * http://fgx/freeflightsim.org 


  (Closed) Developer Group:
<http://fgx-project.googlegroups.com>

  
![image](https://secure.travis-ci.org/fgx/fgx.png?branch=master:target: http://travis-ci.org/fgx/fgx)


Installation
--------------------------
From source
  > cd src/
  > qmake
  > make

About
--------------------------
FGX is a qt-based c++ launcher for FlightGear. There is more than one launcher around for FlightGear like FGrun or others, intention for FGX was mainly to have fast and small launcher for OSX, starting the real and only free and Open Source Flight Simulator FlightGear within 5 seconds (also development versions and nightly builds). Since it makes no sense to have such a launcher only for one platform using qt, it became cross-platform and could be used also with Debian and Win.

The FGX project was started early 2011 by Yves Sablonier in Swiss Mountains, peering to another project started once by Pedro. Bright Pedro in Wales (there are mountains too) was enough of a git to go and check it out, then code a load of patches to its current state and do all needed changes, to get more stable code. Yves Sablonier tried to step down, but Pedro still needs him for issue tracking, polish, to get a good logo, and maybe once for good manuals and documentation. So Yves Sablonier did not step down, but he is always one step behind ...

June 2011 Geoff McLane joined the project and started to extend debugging and logging tools and created a non-database fallback for the xml parsing of aircraft and airport data. In general he brings a lot more style to the code everywhere. He writes proper and important helpers for FGx, like zlib integration or log utilities.

License
------------------------
All the code is under GPLv2 or later. LICENSE.txt and README.txt have to be included with every distribution. Please don't bootleg, make loads of copies and flog them around. Do what you want and what GPL allows you. The logo FGx logo is trademarked and patented under the creative commons and owned by Yves Sablonier, Zurich (unfortunately GPL does not cover free and open source artwork, that's why Yves choose CC, until someone explains him how to protect free artwork under GPL).

Authors
-----------------------
* Pete Morgan (UK) pete [at] freeflightsim dot org
   * Project Lead, Coding lead, Developer and Programmer, Application Design
* Geoff McLane (F) ubuntu [at] geoffair.info
   * Senior Developer, Analyst, Reviewer, Engineer and Programmer
* Yves Sablonier (CH), flightgear [at] sablonier dot ch
   * Sleeping but creator of FGx -  Project Lead, Code Facility Services, GUI Design and Features
   
Some included code in FGx comes from other GPL sources, like zlib from Jean-Loup Gailly/Mark Adler. We try to pay attribution everywhere, you will find such notes directly in the source files or documentation at http://fgx.freeflightsim.org

Thanks from Yves
-------------------------------
First I send my BIG THANKS to Pete, who decided to contribute and who is responsible that FGx improves fast since early 2011. Second my thanks goes to Geoff McLane who brings a lot more style into the code with his analyses, proposals and new code.

And third thanks goes to ALL the FlightGear developers and associated projects. 
Without all this developers, programmers and users it would never be possible 
to have a cool, free, open source Flight Simulator available. 
Special thanks from Yves goes to Martin Spott, James Turner and Frederic Bouvier. 

(Beware of FlightGear versions where you have to pay for, original FlightGear is FREE, has a huge community, 
a living forum and wiki. We guess you will never get a better version than the original.)


FlightGear Links
-----------------------------------
FlightGear Wiki Central:
<http://wiki.flightgear.org>

FlightGear Source, Development Site: 
<http://www.gitorious.org/fg>

FlightGear Forum (User and Development Forums):
<http://www.flightgear.org/forums>


