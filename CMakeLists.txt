# CMakeLists.txt - for FGx launcher project
# CMakeLists.txt, original generated from src\fgx.pro
cmake_minimum_required( VERSION 2.8.8 )

project( fgx )

file(READ version versionFile)
string(STRIP ${versionFile} FGX_VERSION)
string(REPLACE "." ";" VERSION_LIST ${FGX_VERSION})
list(GET VERSION_LIST 0 FGX_MAJOR_VERSION)
list(GET VERSION_LIST 1 FGX_MINOR_VERSION)
list(GET VERSION_LIST 2 FGX_POINT_VERSION)
add_definitions( -DFGX_VERSION="${FGX_VERSION}" )

if (MSVC)
    # to distinguish between debug and release lib
    set( CMAKE_DEBUG_POSTFIX d )
endif ()

# Add LINUX or WINDOWS definitions
if(UNIX)
   add_definitions( -DLINUX )
else(UNIX)
   add_definitions( -DWINDOWS )
endif(UNIX)
# QT = network xml xmlpatterns webkit
# CONFIG = qt thread
message(STATUS "*** Finding Qt4 components ${components}")
find_package ( Qt4 COMPONENTS QtCore QtGui QtNetwork QtWebkit QtXml QtXmlPatterns REQUIRED )
include ( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
# debug messages
message(STATUS "*** include ( ${QT_USE_FILE} )")
message(STATUS "*** defs ${QT_DEFINITIONS}")
message(STATUS "*** libs ${QT_LIBRARIES}")

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTXMLPATTERNS TRUE )
set( QT_USE_QTWEBKIT TRUE )

set( fgx_SRCS
    src/aircraft/aircraftdata.cpp
    src/aircraft/aircraftproxymodel.cpp
    src/aircraft/aircraftwidget.cpp
    src/aircraft/modelinfo.cpp
    src/aircraft/xaero.cpp
    src/airports/airportsdata.cpp
    src/airports/airportswidget.cpp
    src/airports/importairportsdialog.cpp
    src/airports/metarwidget.cpp
    src/fgtools/fileviewerwidget.cpp
    src/launcher/headerwidget.cpp
    src/launcher/launcherwindow.cpp
    src/launcher/menuwidget.cpp
    src/logs/viewlogswidget.cpp
    src/main.cpp
    src/map/openlayerwidget.cpp
    src/mpmap/mpmapwidget.cpp
    src/mpmap/mpmapxwidget.cpp
    src/mpmap/pilotswidget.cpp
    src/mpmap/xwebpage.cpp
    src/network/mptelnet.cpp
    src/network/networkwidget.cpp
    src/panes/coresettingswidget.cpp
    src/panes/expertoptionswidget.cpp
    src/panes/renderingwidget.cpp
    src/panes/timeweatherwidget.cpp
    src/panes/viewlogwidget.cpp
    src/props/propeditwidget.cpp
    src/props/propstreewidget.cpp
    src/props/telnetslave.cpp
    src/settings/settingsdialog.cpp
    src/utilities/dirDialog.cpp
    src/utilities/fgx_gzlib.cpp
    src/utilities/fileDialog.cpp
    src/utilities/helpers.cpp
    src/utilities/loadAptDat.cpp
    src/utilities/messagebox.cpp
    src/utilities/simgear/SGGeodesy.cpp
    src/utilities/statusbar.cpp
    src/utilities/utilities.cpp
    src/utilities/workThread.cpp
    src/utilities/zlib/adler32.c
    src/utilities/zlib/compress.c
    src/utilities/zlib/crc32.c
    src/utilities/zlib/deflate.c
    src/utilities/zlib/gzclose.c
    src/utilities/zlib/gzlib.c
    src/utilities/zlib/gzread.c
    src/utilities/zlib/gzwrite.c
    src/utilities/zlib/infback.c
    src/utilities/zlib/inffast.c
    src/utilities/zlib/inflate.c
    src/utilities/zlib/inftrees.c
    src/utilities/zlib/trees.c
    src/utilities/zlib/uncompr.c
    src/utilities/zlib/zutil.c
    src/xobjects/latlng.cpp
    src/xobjects/mainobject.cpp
    src/xobjects/xopt.cpp
    src/xobjects/xprocess.cpp
    src/xobjects/xsettings.cpp
    src/xobjects/xsettingsmodel.cpp
    src/xwidgets/execontrols.cpp
    src/xwidgets/fgxdebugwidget.cpp
    src/xwidgets/toolbargroup.cpp
    src/xwidgets/xcommandpreviewidget.cpp
    src/xwidgets/xdebugtreewidget.cpp
    src/xwidgets/xgroupboxes.cpp
    )
set( fgx_HDRS
    src/aircraft/aircraftproxymodel.h
    src/aircraft/aircraftwidget.h
    src/aircraft/modelinfo.h
    src/airports/airportswidget.h
    src/airports/importairportsdialog.h
    src/airports/metarwidget.h
    src/fgtools/fileviewerwidget.h
    src/launcher/headerwidget.h
    src/launcher/launcherwindow.h
    src/launcher/menuwidget.h
    src/logs/viewlogswidget.h
    src/map/openlayerwidget.h
    src/mpmap/mpmapwidget.h
    src/mpmap/mpmapxwidget.h
    src/mpmap/pilotswidget.h
    src/mpmap/xwebpage.h
    src/network/mptelnet.h
    src/network/networkwidget.h
    src/panes/coresettingswidget.h
    src/panes/expertoptionswidget.h
    src/panes/renderingwidget.h
    src/panes/timeweatherwidget.h
    src/panes/viewlogwidget.h
    src/props/propeditwidget.h
    src/props/propstreewidget.h
    src/props/telnetslave.h
    src/settings/settingsdialog.h
    src/utilities/dirDialog.h
    src/utilities/fileDialog.h
    src/utilities/loadAptDat.h 
    src/utilities/messagebox.h
    src/utilities/statusbar.h
    src/utilities/workThread.h
    src/xobjects/mainobject.h
    src/xobjects/xprocess.h
    src/xobjects/xsettings.h
    src/xobjects/xsettingsmodel.h
    src/xwidgets/execontrols.h
    src/xwidgets/fgxdebugwidget.h
    src/xwidgets/toolbargroup.h
    src/xwidgets/xcommandpreviewidget.h
    src/xwidgets/xdebugtreewidget.h
    src/xwidgets/xgroupboxes.h
    )
set( fgx_OTHERS
    src/xwidgets/xtreewidgetitem.h
    src/utilities/utilities.h
    src/airports/airportsdata.h
    src/aircraft/aircraftdata.h
    src/utilities/zlib/inftrees.h
    src/utilities/zlib/inflate.h
    src/utilities/zlib/inffixed.h
    src/utilities/zlib/inffast.h
    src/utilities/zlib/gzguts.h
    src/utilities/zlib/fgx_zutil.h
    src/utilities/zlib/fgx_zlib.h
    src/utilities/zlib/fgx_zconf.h
    src/utilities/zlib/deflate.h
    src/utilities/zlib/crc32.h
    src/utilities/zlib/trees.h
    src/utilities/fgx_gzlib.h
    src/utilities/helpers.h
    src/xobjects/latlng.h
    src/xobjects/xopt.h
    src/aircraft/xaero.h
    src/app_config.h
    src/utilities/simgear/SGGeodesy.h
    src/utilities/simgear/constants.h
    src/utilities/simgear/compiler.h
    )
set( fgx_RCS
    src/resources/ycons.qrc
    src/resources/artwork.qrc
    src/resources/fonts.qrc
    src/resources/openlayers.qrc
    src/resources/default.qrc
    src/resources/images.qrc
    )
include_directories(
    src/utilities/zlib
    src/xwidgets
    src/logs
    src/aircraft
    src/launcher
    src/airports
    src/panes
    src/map
    src src/xobjects
    src/utilities/simgear
    src/props
    src/network
    src/resources
    src/utilities
    src/settings src/mpmap
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QT_INCLUDE_DIR}
 )

# Added for DEBUG only
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "*** inc_dirs = ${inc_dirs}")

QT4_WRAP_CPP( fgx_HDRS_MOC ${fgx_HDRS} )
QT4_ADD_RESOURCES( fgx_RESOURCES_RCC ${fgx_RCS} )

add_executable( fgx ${fgx_SRCS} ${fgx_HDRS_MOC} ${fgx_RESOURCES_RCC} ${fgx_OTHERS} )

target_link_libraries( fgx ${QT_LIBRARIES} )
if (MSVC)
    set_target_properties( fgx PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install
install(TARGETS fgx DESTINATION bin )

##=====================================================
## === Debian - pete checking things out !
IF(WIN32)
   set(CPACK_GENERATOR "NSIS;ZIP")
else(WIN32)
   set(CPACK_GENERATOR "RPM;DEB")
endif(WIN32)

set(CPACK_PACKAGE_VENDOR "FGx Team")
set(CPACK_PACKAGE_CONTACT "fgx@freeflightsim.org")

set(CPACK_PACKAGE_VERSION ${FGX_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${FGX_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${FGX_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${FGX_POINT_VERSION}")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FGx FlightGear launcher")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/etc/dist_description.txt")
#set(CPACK_PACKAGE_DESCRIPTION "FGx - A cross-platform FlightGear launcherXXX")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")


## DEB Specific
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_CONTACT})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://fgx.freeflightsim.org")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-xml,libqt4-network,libqt4-gui,libqt4-webkit")
set(CPACK_DEBIAN_PACKAGE_SECTION "Games")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")


## NSIS specific
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/src/resources/fgx.ico")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
set(CPACK_NSIS_HELP_LINK "http://fgx.freeflightsim.org")
set(CPACK_NSIS_URL_INFO_ABOUT "http://fgx.freeflightsim.org")

if(UNIX)
    ##  logo
    install(FILES "${CMAKE_SOURCE_DIR}/src/resources/fgx_48x48x32.png"
            DESTINATION "/usr/share/pixmaps"
    )
    ## ubuntu shortcut
    install(FILES "${CMAKE_SOURCE_DIR}/etc/fgx.desktop"
            DESTINATION "/usr/share/applications"
    )
ENDIF(UNIX)

INCLUDE(CPack)

# EOF
