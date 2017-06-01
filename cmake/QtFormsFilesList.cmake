#
#	This file defines functions to build a list of Qt forms files to be used to create libraries and executables, and install files, respectively.
#
#	Call add_forms with one or more forms files to add them to the list of files.
#	Call preprocess_forms to generate a list of files to include in a library or executable.
#	Use the PREP_FORMS variable to include the list.
#	Call create_forms_groups with the root directory for the files in the list after defining the library or executable to generate filters for Visual Studio that match the directory structure.
#	Call clear_forms to clear the forms list and prepare it for the next library or executable.
#

# function to collect all the sources from sub-directories
# into a single list
function(add_forms)
  get_property(is_defined GLOBAL PROPERTY FORMS_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY FORMS_LIST
      BRIEF_DOCS "List of forms files"
      FULL_DOCS "List of forms files to be compiled in one library")
  endif()
  # make absolute paths
  set(FORMS)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND FORMS "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY FORMS_LIST "${FORMS}")
endfunction(add_forms)

# preprocess forms
function( preprocess_forms )
	set(PREP_FORMS PARENT_SCOPE)
	get_property(SRCS GLOBAL PROPERTY FORMS_LIST)
	foreach(s IN LISTS SRCS)
	  file(RELATIVE_PATH rs "${CMAKE_CURRENT_SOURCE_DIR}" "${s}")
	  string(REGEX REPLACE "r$" "" o "${CMAKE_CURRENT_BINARY_DIR}/${rs}")
	  add_custom_command(
		OUTPUT "${o}"
		COMMAND ${CMAKE_COMMAND} -E copy "${s}" "${o}"
		DEPENDS "${s}"
		COMMENT "Creating ${o}"
		VERBATIM
		)
	  list(APPEND PREP_FORMS "${s}")
	endforeach()
	set(PREP_FORMS ${PREP_FORMS} PARENT_SCOPE)
endfunction( preprocess_forms )

#Creates the source groups for forms.
#If _root_group is not empty, it is used as the root source group for forms
function( create_forms_groups _src_root_path _root_group )
	get_property(SRCS GLOBAL PROPERTY FORMS_LIST)

	#Append a slash if needed
	if( NOT _root_group STREQUAL "" AND NOT ${_root_group} MATCHES ".*\\$" )
		set( _root_group "${_root_group}\\" )
	endif()
	
	foreach(_source IN ITEMS ${SRCS})
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_root_group}${_group_path}" FILES "${_source}")
	endforeach()
endfunction( create_forms_groups )

#Function to clear the forms list.
function( clear_forms )
	set_property( GLOBAL PROPERTY FORMS_LIST "" )
	set( PREP_FORMS PARENT_SCOPE )
endfunction( clear_forms )

