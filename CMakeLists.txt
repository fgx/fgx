# 20131203 - single root CMakeLists.txt - now MANUALLY maintained
# original CMakeLists.txt, generated from [F:\FG\18\fgx\src\fgx.pro]

cmake_minimum_required( VERSION 2.6 )

# We have some custom .cmake scripts not in the official distribution.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
message(STATUS "*** Established modules path ${CMAKE_MODULE_PATH}")

project( fgx )

option(SYSTEM_ZLIB "Use the system version of Zlib" OFF)

# Added for DEBUG only
if(UNIX)
  if(APPLE)
    set(GUI "Cocoa")
  else()
    set(GUI "X11")
  endif()
else()
  if(WIN32)
    set(GUI "Win32")
  else()
    set(GUI "Unknown")
  endif()
endif()
message(STATUS "*** GUI system is ${GUI} ***")

# to distinguish between debug and release lib
if (WIN32)
    set( CMAKE_DEBUG_POSTFIX "d" )
endif ()

# Setup MSVC 3rd party directories
include( ConfigureMsvc3rdParty )

# Add LINUX or WINDOWS definitions - is this still required?
if(UNIX)
   add_definitions( -DLINUX )
else()
   add_definitions( -DWINDOWS )
endif()

# CMAKE_EXE_LINKER_FLAGS: Linker flags used to create executables
if (WIN32)
    if (MSVC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
        add_definitions( -D_USE_MATH_DEFINES ) # for M_PI in <math.h>
    endif ()
endif ()

# QT = network xml xmlpatterns webkit script
# CONFIG = qt thread
message(STATUS "*** Finding Qt4 components ${compponents}")
find_package ( Qt4 COMPONENTS QtCore QtGui QtNetwork QtWebkit QtXml QtXmlPatterns QtScript QtScriptTools REQUIRED )
include ( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
# debug messages
message(STATUS "*** include ( ${QT_USE_FILE} )")
message(STATUS "*** add_definitions( ${QT_DEFINITIONS} )")

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTXMLPATTERNS TRUE )
set( QT_USE_QTWEBKIT TRUE )
set( QT_USE_QTSCRIPT TRUE )

# =========================================================
#  MARBLE_FOUND - system has Marble
#  MARBLE_INCLUDE_DIR - the Marble include directory
#  MARBLE_LIBRARIES 
#find_package( Marble REQUIRED )
find_package( Marble )
if (MARBLE_FOUND)
    include_directories( ${MARBLE_INCLUDE_DIR} )
    list( APPEND extra_LIBS ${MARBLE_LIBRARIES} )
    message(STATUS "*** Found Marble libs ${MARBLE_LIBRARIES} incs ${MARBLE_INCLUDE_DIR}")
else ()
    message(FATAL_ERROR "*** Marble NOT found. Set ENV MARBLE_ROOT if installed in a special location")
endif ()
# ============================================================

# =========================================================
#  ZLIB_FOUND - system has ZLIB
#  ZLIB_INCLUDE_DIR - the ZLIB include directory
#  ZLIB_LIBRARIES
if (SYSTEM_ZLIB)
    find_package( ZLIB )
    if (ZLIB_FOUND)
        include_directories( ${ZLIB_INCLUDE_DIR} )
        list( APPEND extra_LIBS ${ZLIB_LIBRARIES} )
        message(STATUS "Using ZLIB includes at: ${ZLIB_INCLUDE_DIR}")
        message(STATUS "Using ZLIB libraries: ${ZLIB_LIBRARIES}")
        add_definitions(-DUSE_SYSTEM_ZLIB)
    endif ()
endif ()
# ============================================================

set( fgx_SRCS src/main.cpp src/xobjects/xsettings.cpp src/settings/settingsdialog.cpp src/network/networkwidget.cpp
    src/network/mptelnet.cpp src/aircraft/aircraftwidget.cpp src/airports/airportswidget.cpp
    src/xwidgets/execontrols.cpp src/panes/timeweatherwidget.cpp src/panes/renderingwidget.cpp
    src/panes/coresettingswidget.cpp src/xwidgets/xgroupboxes.cpp src/xobjects/mainobject.cpp
    src/launcher/launcherwindow.cpp src/utilities/statusbar.cpp src/utilities/utilities.cpp
    src/utilities/messagebox.cpp src/airports/importairportsdialog.cpp src/airports/airportsdata.cpp
    src/aircraft/aircraftdata.cpp src/panes/viewlogwidget.cpp src/utilities/fgx_gzlib.cpp
    src/utilities/helpers.cpp src/xobjects/xprocess.cpp src/props/telnetslave.cpp src/props/propstreewidget.cpp
    src/logs/viewlogswidget.cpp src/xobjects/latlng.cpp src/props/propeditwidget.cpp src/mpmap/mpmapwidget.cpp
    src/mpmap/flightswidget.cpp src/mpmap/xwebpage.cpp src/panes/expertoptionswidget.cpp
    src/xobjects/xsettingsmodel.cpp src/xwidgets/fgxdebugwidget.cpp src/launcher/headerwidget.cpp
    src/xwidgets/xcommandpreviewidget.cpp src/xobjects/xopt.cpp src/xwidgets/xdebugtreewidget.cpp
    src/map/openlayerwidget.cpp src/aircraft/xaero.cpp src/airports/metarwidget.cpp src/utilities/fileDialog.cpp
    src/utilities/dirDialog.cpp src/launcher/menuwidget.cpp src/utilities/workThread.cpp
    src/utilities/loadAptDat.cpp src/utilities/simgear/SGGeodesy.cpp src/xmarble/mapmainwindow.cpp
    src/xmarble/mappanel.cpp src/xmarble/mapcorewidget.cpp src/xmarble/xmarblewidget.cpp
    src/network/crossfeedjson.cpp
    src/flights/flightsmodel.cpp src/flights/flightpositions.cpp
    src/xwidgets/toolbargroup.cpp
    src/navdata/navdatawidget.cpp src/navdata/navdatamodel.cpp src/xmarble/mapviewsmodel.cpp
    src/xmarble/mapselectdialog.cpp src/xmarble/flightspaintlayer.cpp )

set( fgx_HDRS src/xobjects/xsettings.h src/settings/settingsdialog.h src/network/networkwidget.h
    src/network/mptelnet.h src/aircraft/aircraftwidget.h src/airports/airportswidget.h
    src/xwidgets/execontrols.h src/panes/timeweatherwidget.h src/panes/renderingwidget.h
    src/panes/coresettingswidget.h src/xwidgets/xgroupboxes.h src/xobjects/mainobject.h
    src/launcher/launcherwindow.h src/utilities/statusbar.h src/utilities/messagebox.h
    src/airports/importairportsdialog.h src/panes/viewlogwidget.h src/xobjects/xprocess.h
    src/props/telnetslave.h src/props/propstreewidget.h src/logs/viewlogswidget.h src/props/propeditwidget.h
    src/mpmap/mpmapwidget.h src/mpmap/flightswidget.h src/mpmap/xwebpage.h src/panes/expertoptionswidget.h
    src/xobjects/xsettingsmodel.h src/xwidgets/fgxdebugwidget.h src/launcher/headerwidget.h
    src/xwidgets/xcommandpreviewidget.h src/xwidgets/xdebugtreewidget.h src/map/openlayerwidget.h
    src/airports/metarwidget.h src/utilities/fileDialog.h src/utilities/dirDialog.h src/launcher/menuwidget.h
    src/utilities/workThread.h src/utilities/loadAptDat.h src/xmarble/mapmainwindow.h
    src/xmarble/mappanel.h src/xmarble/mapcorewidget.h src/network/crossfeedjson.h
    src/flights/flightsmodel.h src/flights/flightpositions.h
    src/xwidgets/toolbargroup.h src/navdata/navdatawidget.h src/navdata/navdatamodel.h
    src/xmarble/mapviewsmodel.h src/xmarble/mapselectdialog.h src/xmarble/flightspaintlayer.h )

set( fgx_OTHERS src/xwidgets/xtreewidgetitem.h src/utilities/utilities.h src/airports/airportsdata.h
    src/aircraft/aircraftdata.h
    src/utilities/fgx_gzlib.h src/utilities/helpers.h src/xobjects/latlng.h src/xobjects/xopt.h
    src/aircraft/xaero.h src/app_config.h src/utilities/simgear/SGGeodesy.h src/utilities/simgear/constants.h
    src/utilities/simgear/compiler.h src/xmarble/xmarblewidget.h
    src/flights/flightsmodel.h src/flights/flightpositions.h )

set( fgx_RCS src/resources/ycons.qrc src/resources/artwork.qrc src/resources/fonts.qrc src/resources/openlayers.qrc
    src/resources/default.qrc src/resources/images.qrc src/resources/marble_icons.qrc
    )

if (NOT ZLIB_FOUND)
    message(STATUS "Using local version of Zlib")
    list( APPEND fgx_SRCS src/utilities/zlib/uncompr.c src/utilities/zlib/trees.c src/utilities/zlib/inftrees.c
    src/utilities/zlib/inflate.c src/utilities/zlib/inffast.c src/utilities/zlib/infback.c
    src/utilities/zlib/gzwrite.c src/utilities/zlib/gzread.c src/utilities/zlib/gzlib.c
    src/utilities/zlib/gzclose.c src/utilities/zlib/deflate.c src/utilities/zlib/crc32.c
    src/utilities/zlib/compress.c src/utilities/zlib/adler32.c src/utilities/zlib/zutil.c )
    list( APPEND fgx_OTHERS src/utilities/zlib/inftrees.h src/utilities/zlib/inflate.h
    src/utilities/zlib/inffixed.h src/utilities/zlib/inffast.h src/utilities/zlib/gzguts.h
    src/utilities/zlib/fgx_zutil.h src/utilities/zlib/fgx_zlib.h src/utilities/zlib/fgx_zconf.h
    src/utilities/zlib/deflate.h src/utilities/zlib/crc32.h src/utilities/zlib/trees.h )
    include_directories( src/utilities/zlib )
endif ()

include_directories( src/xwidgets src/logs src/aircraft src/launcher src/xmarble src/airports
    src/panes src/map src src/xobjects src/utilities/simgear src/props src/network src/resources
    src/utilities src/navdata src/settings src/mpmap
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QT_INCLUDE_DIR}
    )

# Added for DEBUG only
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "*** inc_dirs = ${inc_dirs}")

QT4_WRAP_CPP( fgx_HDRS_MOC ${fgx_HDRS} )
QT4_ADD_RESOURCES( fgx_RESOURCES_RCC ${fgx_RCS} )

add_executable( fgx ${fgx_SRCS} ${fgx_HDRS_MOC} ${fgx_RESOURCES_RCC} ${fgx_OTHERS} )
target_link_libraries( fgx ${QT_LIBRARIES} ${extra_LIBS} )
if (WIN32)
    set_target_properties( fgx PROPERTIES DEBUG_POSTFIX d )
endif ()

# deal with install 
install(TARGETS fgx DESTINATION bin )

# eof - original generated by Qt2cmake.pl, on 2013/12/02 14:18:51
