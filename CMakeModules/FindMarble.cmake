# 20131202 - imported into FGx project
# Try to find the Marble Library
# Once done this will define
#
#  MARBLE_FOUND - system has Marble
#  MARBLE_INCLUDE_DIR - the Marble include directory
#  MARBLE_LIBRARIES 
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if ( MARBLE_INCLUDE_DIR AND MARBLE_LIBRARIES )
   # in cache already
   set( MARBLE_FIND_QUIETLY TRUE )
endif ()

find_path( MARBLE_INCLUDE_DIR NAMES marble/MarbleModel.h
    HINTS $ENV{MARBLE_ROOT}
    PATH_SUFFIXES include )
find_library( MARBLE_LIBRARY_DEBUG 
    NAMES marblewidgetd
    HINTS $ENV{MARBLE_ROOT}
    PATH_SUFFIXES lib )
find_library( MARBLE_LIBRARY_RELEASE 
    NAMES marblewidget
    HINTS $ENV{MARBLE_ROOT}
    PATH_SUFFIXES lib )
if (MARBLE_LIBRARY_DEBUG AND MARBLE_LIBRARY_RELEASE)
    set(MARBLE_LIBRARIES
        optimized ${MARBLE_LIBRARY_RELEASE}
        debug ${MARBLE_LIBRARY_DEBUG} )
else ()
    set(MARBLE_LIBRARIES ${MARBLE_LIBRARY_RELEASE})
endif ()

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( marble DEFAULT_MSG MARBLE_INCLUDE_DIR MARBLE_LIBRARIES )

# eof
