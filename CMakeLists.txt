###################################################
#                                                 #
#                                                 #
#   Angelscript IDE CMake build file              #
#                                                 #
#                                                 #
###################################################
cmake_minimum_required( VERSION 3.6 )

include( cmake/InputFilesList.cmake )
include( cmake/QtFormsFilesList.cmake )
include( cmake/WinXPSupport.cmake )

project( AngelscriptIDE )

check_winxp_support()

set(variables
  CMAKE_C_FLAGS_DEBUG
  CMAKE_C_FLAGS_MINSIZEREL
  CMAKE_C_FLAGS_RELEASE
  CMAKE_C_FLAGS_RELWITHDEBINFO
  CMAKE_CXX_FLAGS_DEBUG
  CMAKE_CXX_FLAGS_MINSIZEREL
  CMAKE_CXX_FLAGS_RELEASE
  CMAKE_CXX_FLAGS_RELWITHDEBINFO
)
message(STATUS "Initial build flags:")
foreach(variable ${variables})
  message(STATUS "  '${variable}': ${${variable}}")
endforeach()
message(STATUS "")

set( QT_CMAKE_PREFIX "" CACHE PATH "Path to Qt directory, e.g. C:/Qt/5.9/msvc2015. Required to find Qt dependencies" )

set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_CMAKE_PREFIX} )

# Find includes in corresponding build directories
set( CMAKE_INCLUDE_CURRENT_DIR ON )
# Instruct CMake to run moc automatically when needed.
set( CMAKE_AUTOMOC ON )

# Find the QtWidgets library
find_package( Qt5Widgets )

#C++14 support
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y" )

if( UNIX )
	#Additional debug info for GDB.
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
endif()

set( OUTPUT_DIR "${CMAKE_SOURCE_DIR}/test" CACHE PATH "Output directory for binaries" )

set( EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external )
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set( SHARED_INCLUDE_DIRECTORIES )
set( SHARED_DEFS )
set( SHARED_LIBS )

if( MSVC )
	set( LINUX_32BIT_FLAG "" )
else()
	set( LINUX_32BIT_FLAG "-m32" )
endif()

set( SHARED_LINKER_FLAGS )

#Copy dependent libraries
set( QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui )

set( COPY_QT_DEBUG_LIBS OFF CACHE BOOL "Whether to copy Qt debug libraries" )

foreach( lib_name IN LISTS QT_LIBS )
	get_target_property( lib ${lib_name} LOCATION )
	
	message( STATUS "Copying library \"${lib}\"" )
	file( COPY ${lib} DESTINATION ${OUTPUT_DIR} )
	
	if( COPY_QT_DEBUG_LIBS )
		#Copy the debug version as well
		#TODO: probably doesn't work on Linux, should figure out how to automate this
		get_filename_component( FILE_DIR ${lib} DIRECTORY )
		get_filename_component( FILE_NAME ${lib} NAME_WE )
		get_filename_component( FILE_EXT ${lib} EXT )
		set( DEBUG_LIB ${FILE_DIR}/${FILE_NAME}d${FILE_EXT} )
		
		message( STATUS "Copying library \"${DEBUG_LIB}\"" )
		file( COPY ${DEBUG_LIB} DESTINATION ${OUTPUT_DIR} )
	endif()
endforeach()

#Add Angelscript library
add_library( Angelscript STATIC IMPORTED )
set_property( TARGET Angelscript PROPERTY IMPORTED_LOCATION "${EXTERNAL_DIR}/Angelscript/lib/Angelscript.lib" )
set_property( TARGET Angelscript PROPERTY IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/Angelscript/lib/Angelscriptd.lib" )

#Add AngelscriptUtils library
add_library( AngelscriptUtils STATIC IMPORTED )
set_property( TARGET AngelscriptUtils PROPERTY IMPORTED_LOCATION "${EXTERNAL_DIR}/Angelscript/lib/AngelscriptUtils.lib" )
set_property( TARGET AngelscriptUtils PROPERTY IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/Angelscript/lib/AngelscriptUtilsd.lib" )

add_subdirectory( src )
