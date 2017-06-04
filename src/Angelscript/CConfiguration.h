#ifndef ANGELSCRIPT_CCONFIGURATION_H
#define ANGELSCRIPT_CCONFIGURATION_H

#include <string>
#include <vector>

/**
*	Represents one configuration
*/
class CConfiguration
{
public:
	CConfiguration( const std::string& szName );
	~CConfiguration() = default;

	CConfiguration( const CConfiguration& other ) = default;
	CConfiguration& operator=( const CConfiguration& other ) = default;

	const std::string& GetName() const { return m_szName; }

	/**
	*	Sets the name of this configuration
	*	Renaming configurations is only valid through the configuration manager
	*/
	void SetName( const std::string& szName );

	const std::string& GetConfigFilename() const { return m_szConfigFilename; }

	void SetConfigFilename( const std::string& szConfigFilename );

	const std::string& GetIncludeFilename() const { return m_szIncludeFilename; }

	void SetIncludeFilename( const std::string& szIncludeFilename );

	const std::string& GetFallbackExtension() const { return m_szFallbackExtension; }

	void SetFallbackExtension( const std::string& szFallbackExtension );

	const std::vector<std::string>& GetWords() const { return m_Words; }
	std::vector<std::string>& GetWords() { return m_Words; }

	const std::vector<std::string>& GetIncludePaths() const { return m_IncludePaths; }
	std::vector<std::string>& GetIncludePaths() { return m_IncludePaths; }

	const std::vector<std::string>& GetExtensions() const { return m_Extensions; }
	std::vector<std::string>& GetExtensions() { return m_Extensions; }

private:
	std::string m_szName;								//Name of this configuration, without extension or path
	std::string m_szConfigFilename;						//Complete path and filename of a configuration file used to initialize the Angelscript API
	std::string m_szIncludeFilename;					//Name of a file to include in all files being compiled
	std::string m_szFallbackExtension;					//Extension to add when searching for #include'd files with no extension given

	std::vector<std::string> m_Words;					//Preprocessor words to define
	std::vector<std::string> m_IncludePaths;			//Include paths to consider when searching for #include'd files.
														//Paths starting with '.' are considered to be relative paths

	std::vector<std::string> m_Extensions;				//List of extensions to consider when opening or saving files
};

#endif //ANGELSCRIPT_CCONFIGURATION_H
