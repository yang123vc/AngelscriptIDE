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

add_subdirectory( ${EXTERNAL_DIR}/Angelscript Angelscript_lib )

preprocess_sources()
preprocess_forms()

qt5_wrap_ui( UI_SRCS ${PREP_FORMS} )

add_executable( ${PROJECT_NAME} WIN32 ${PREP_SRCS} ${UI_SRCS} )

target_include_directories( ${PROJECT_NAME} PRIVATE
	${EXTERNAL_DIR}/Angelscript
	${EXTERNAL_DIR}/Angelscript/include
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
