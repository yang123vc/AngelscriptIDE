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

project( AngelscriptIDE )

include( cmake/WinXPSupport.cmake )

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

set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

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

#
#Angelscript IDE
#

set( PROJECT_NAME AngelscriptIDE )

#Add in the directories
add_subdirectory( Angelscript )
add_subdirectory( ide )
add_subdirectory( launcher )
add_subdirectory( main )
add_subdirectory( ui )
add_subdirectory( util )

preprocess_sources()
preprocess_forms()

qt5_wrap_ui( UI_SRCS ${PREP_FORMS} )

add_executable( ${PROJECT_NAME} WIN32 ${PREP_SRCS} ${UI_SRCS} )

target_include_directories( ${PROJECT_NAME} PRIVATE
	${SRC_DIR}/Angelscript/include
	${SHARED_INCLUDE_DIRECTORIES}
)

target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${SHARED_DEFS}
)

#Find dependencies

#Link with dependencies
target_link_libraries( ${PROJECT_NAME} 
	Qt5::Widgets
	${SHARED_LIBS}
)

#CMake places libraries in /Debug or /Release on Windows, so explicitly set the paths for both.
#On Linux, it uses LIBRARY_OUTPUT_DIRECTORY
set_target_properties( ${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_MINSIZEREL "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_RELWITHDEBINFO "${OUTPUT_DIR}"
)

set( PROJECT_LINK_FLAGS )

if( NOT MSVC AND NOT APPLE )
	#Generate a map file that provides information about the linking stage.
	set( PROJECT_LINK_FLAGS
		${PROJECT_LINK_FLAGS} "-Wl,-Map,${PROJECT_NAME}_map.txt "
	)
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES
	COMPILE_FLAGS "${LINUX_32BIT_FLAG}"
	LINK_FLAGS "${SHARED_LINKER_FLAGS} ${PROJECT_LINK_FLAGS} ${LINUX_32BIT_FLAG}"
)

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )
create_forms_groups( "${CMAKE_SOURCE_DIR}" "Forms" )

clear_sources()
clear_forms()

#
#End exe
#
