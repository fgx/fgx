#================================================
# CMakeLists.txt - for FGx launcher project
#================================================
# This project is not built and managed by cmake..
# sorry folks but qmake dont work xplatform.. the way we want
#

cmake_minimum_required( VERSION 2.8.8 )

project( fgx )

#=====================================================
## Set Version  from ./version txt file
file(READ version versionFile)
string(STRIP ${versionFile} APP_VERSION)
string(REPLACE "." ";" VERSION_LIST ${APP_VERSION})
list(GET VERSION_LIST 0 APP_MAJOR_VERSION)
list(GET VERSION_LIST 1 APP_MINOR_VERSION)
list(GET VERSION_LIST 2 APP_POINT_VERSION)

#=======================================================
## Setup  APP macros
# This block assign values for final use in application
# the definitions such as as source links contact are here

add_definitions( -DAPP_VERSION="${APP_VERSION}" )

set(APP_SHORT "FGx")
add_definitions( -DAPP_SHORT="${APP_SHORT}" )

set(APP_ONE_LINER "FGX - cross-plafrom launcher for FlightGear")
add_definitions( -DAPP_ONE_LINER="${APP_ONE_LINER}" )

set(APP_ORG "FGx Team")
add_definitions( -DAPP_ORG="${APP_ORG}" )

set(APP_DOMAIN "fgx.freeflightsim.org")
add_definitions( -DAPP_DOMAIN="${APP_DOMAIN}" )

set(APP_CONTACT "fgx@freeflightsim.org")
add_definitions( -DAPP_CONTACT="${APP_CONTACT}" )

set(APP_WWW "http://${APP_DOMAIN}")
add_definitions( -DAPP_WWW="${APP_WWW}" )

set(APP_PROJECT "http://github.com/fgx/fgx")
add_definitions( -DAPP_PROJECT="${APP_PROJECT}" )

set(APP_ISSUES "http://github.com/fgx/fgx/issues")
add_definitions( -DAPP_ISSUES="${APP_ISSUES}" )

#=====================================================
## Set up Platform

# probably unix only
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

# determine 32/64 bit
set(BITNESS 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(BITNESS 64)
endif()


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


#=====================================================
## QT
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


include_directories("/usr/include/subversion-1" "/usr/include/svncpp" "/usr/include/apr-1.0")

#=====================================================
## Sources
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
    src/installer/installwindow.cpp
    )
#=====================================================
## Headers
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
    src/installer/installwindow.h
    )
#=====================================================
## Others..
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

#=====================================================
## Resource files
set( fgx_RCS
    src/resources/ycons.qrc
    src/resources/artwork.qrc
    src/resources/fonts.qrc
    src/resources/openlayers.qrc
    src/resources/default.qrc
    )

#=====================================================
## Directories
include_directories(
    src/utilities/zlib
    src/xwidgets
    src/logs
    src/aircraft
    src/launcher
    src/installer
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

#=====================================================
## Setup targets

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


#==============================================================
# Determine which branch we on

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

else(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(GIT_BRANCH "")

endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

message(STATUS "*** Git current branch: ${GIT_BRANCH}")
if( GIT_BRANCH STREQUAL "next" )
    set(APP_RELEASE "unstable")
else(GIT_BRANCH STREQUAL "next")
    set(APP_RELEASE "stable")
endif(GIT_BRANCH STREQUAL "next")
message(STATUS "*** app release: ${APP_RELEASE}")

##======================================================================
## cpack - Packaging and Installers
##======================================================================
## What we probably want
# fgx-VER-BITNESSbit-{platform}-{release}.deb/rpm

IF(WIN32)
   set(CPACK_GENERATOR "NSIS;ZIP")
else(WIN32)
   set(CPACK_GENERATOR "DEB;RPM")
endif(WIN32)

set(CPACK_PACKAGE_VENDOR ${APP_ORG})
set(CPACK_PACKAGE_CONTACT ${APP_CONTACT})

set(CPACK_PACKAGE_VERSION ${APP_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${APP_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${APP_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${APP_POINT_VERSION}")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${APP_ONE_LINER}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/etc/dist_description.txt")
set(CPACK_PACKAGE_DESCRIPTION ${APP_POINT_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")


## DEB Specific
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_CONTACT})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${APP_WWW})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-xml,libqt4-network,libqt4-gui,libqt4-webkit,libqt4-xmlpatterns")
set(CPACK_DEBIAN_PACKAGE_SECTION "Games")


## NSIS specific
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/src/resources/fgx.ico")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
set(CPACK_NSIS_HELP_LINK ${APP_WWW})
set(CPACK_NSIS_URL_INFO_ABOUT ${APP_WWW})


if(UNIX)
    set( CPACK_PACKAGE_FILE_NAME "fgx-${CPACK_PACKAGE_VERSION}-linux-${BITNESS}bit-${APP_RELEASE}" )
    ##  logo
    install(FILES "${CMAKE_SOURCE_DIR}/src/resources/fgx_48x48x32.png"
            DESTINATION "/usr/share/pixmaps"
    )
    ## ubuntu shortcut
    install(FILES "${CMAKE_SOURCE_DIR}/etc/fgx.desktop"
            DESTINATION "/usr/share/applications"
    )
else()
    set( CPACK_PACKAGE_FILE_NAME "fgx-${CPACK_PACKAGE_VERSION}-${ARCHITECTURE}" )
ENDIF(UNIX)

INCLUDE(CPack)


# EOF
