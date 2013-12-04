# 20130505 - ConfigureMsvc3rdParty.cmake - Configure 3rd Party Library Paths on Windows

if (MSVC)
    get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} PATH)
    message(STATUS "*** Using PARENT_DIR=${PARENT_DIR}")
    if (CMAKE_CL_64)
        SET(TEST_3RDPARTY_DIR "${PARENT_DIR}/3rdparty.x64")
    else ()
        SET(TEST_3RDPARTY_DIR "${PARENT_DIR}/3rdparty")
    endif ()
    message(STATUS "*** Testing TEST_3RDPARTY_DIR=${TEST_3RDPARTY_DIR}")
    if (EXISTS ${TEST_3RDPARTY_DIR})
        #set(MSVC_3RDPARTY_ROOT ${PARENT_DIR} CACHE PATH "Location where the third-party dependencies are extracted")
        set(MSVC_3RDPARTY_ROOT ${PARENT_DIR})
        message(STATUS "*** Found MSVC_3RDPARTY_ROOT=${MSVC_3RDPARTY_ROOT}")
    else ()
        #set(MSVC_3RDPARTY_ROOT NOT_FOUND CACHE PATH "Location where the third-party dependencies are extracted")
        set(MSVC_3RDPARTY_ROOT NOT_FOUND)
        message(STATUS "*** NOT FOUND TEST_3RDPARTY_DIR=${TEST_3RDPARTY_DIR}")
    endif ()
    list(APPEND PLATFORM_LIBS "winmm.lib")
else ()
    set(MSVC_3RDPARTY_ROOT NOT_FOUND CACHE PATH "Location where the third-party dependencies are extracted")
endif ()

if (MSVC AND MSVC_3RDPARTY_ROOT)
    message(STATUS "*** 3rdparty files located in ${MSVC_3RDPARTY_ROOT}")
    set( OSG_MSVC "msvc" )
    if (${MSVC_VERSION} EQUAL 1700)
        set( OSG_MSVC ${OSG_MSVC}110 )
    elseif (${MSVC_VERSION} EQUAL 1600)
        set( OSG_MSVC ${OSG_MSVC}100 )
    else ()
        set( OSG_MSVC ${OSG_MSVC}90 )
    endif ()
    if (CMAKE_CL_64)
        set( OSG_MSVC ${OSG_MSVC}-64 )
        set( MSVC_3RDPARTY_DIR 3rdParty.x64 )
		set( BOOST_LIB lib64 )
    else ()
        set( MSVC_3RDPARTY_DIR 3rdParty )
		set( BOOST_LIB lib )
    endif ()
    #find_path(FLTK_DIR include/FL/Fl.H
	#		${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/fltk
	#		${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/fltk-1.3
	#		${MSVC_3RDPARTY_ROOT}/${MSVC_3RDPARTY_DIR} )
	# list (APPEND CMAKE_PROGRAM_PATH ${FLTK_DIR}/bin)
    # ${FLTK_DIR}/lib ${FLTK_DIR}/include
    # ${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/OpenScenegraph/lib ${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/OpenRTI/lib ${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/SimGear/lib $(BOOST_ROOT)/$(BOOST_LIB)
    # ${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/OpenScenegraph/include ${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/OpenRTI/include ${MSVC_3RDPARTY_ROOT}/install/${OSG_MSVC}/SimGear/include
    set (CMAKE_LIBRARY_PATH ${MSVC_3RDPARTY_ROOT}/${MSVC_3RDPARTY_DIR}/lib  ${CMAKE_LIBRARY_PATH})
    set (CMAKE_INCLUDE_PATH ${MSVC_3RDPARTY_ROOT}/${MSVC_3RDPARTY_DIR}/include ${CMAKE_INCLUDE_PATH})
    #find_path(BOOST_ROOT boost/version.hpp
	#		${MSVC_3RDPARTY_ROOT}/boost
	#		${MSVC_3RDPARTY_ROOT}/boost_1_53_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_52_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_51_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_50_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_49_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_48_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_47_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_46_1
	#		${MSVC_3RDPARTY_ROOT}/boost_1_46_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_45_0
	#		${MSVC_3RDPARTY_ROOT}/boost_1_44_0 )
    #message(STATUS "BOOST_ROOT is ${BOOST_ROOT}")
    #set (OPENAL_INCLUDE_DIR ${MSVC_3RDPARTY_ROOT}/${MSVC_3RDPARTY_DIR}/include)
    #set (OPENAL_LIBRARY_DIR ${MSVC_3RDPARTY_ROOT}/${MSVC_3RDPARTY_DIR}/lib)
endif ()

# eof
