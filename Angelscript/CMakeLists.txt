add_sources(
	CScript.cpp
	CScript.h
	CScriptFile.cpp
	CScriptFile.h
	CConfiguration.cpp
	CConfiguration.h
	ConfigurationException.h
	IConfigurationManager.h
	IConfigurationEventListener.h
	CASInstance.cpp
	CASInstance.h
	CASManager.cpp
	CASManager.h
	IASEventListener.h
	IASCompilerListener.h
)

add_subdirectory( add_on )
add_subdirectory( include )
add_subdirectory( source )
