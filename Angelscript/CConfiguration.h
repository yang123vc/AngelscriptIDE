#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

#include <memory>
#include <string>
#include <vector>

class IConfigurationManager;

class CConfiguration : public std::enable_shared_from_this<CConfiguration>
{
public:
	static const char* CONFIG_DIRECTORY;
	static const char* CONFIG_FILE_TYPE;

	static const char WORD_DELIMITER			= ';';
	static const char INCLUDEPATH_DELIMITER		= ';';
	static const char EXTENSION_DELIMITER		= ';';

public:
	CConfiguration( std::shared_ptr<IConfigurationManager> manager, const std::string& szName, const std::string& szConfigFilename = "" );
	~CConfiguration() = default;

	CConfiguration( const CConfiguration& other ) = default;
	CConfiguration& operator=( const CConfiguration& other ) = default;

	static std::string MakeConfigurationPath( const std::string& szName );

	const std::string& GetName() const { return m_szName; }

	void SetName( const std::string& szName );

	const std::string& GetConfigFilename() const { return m_szConfigFilename; }

	void SetConfigFilename( const std::string& szConfigFilename );

	const std::string& GetIncludeFilename() const { return m_szIncludeFilename; }

	void SetIncludeFilename( const std::string& szIncludeFilename );

	std::vector<std::string>& GetWords() { return m_Words; }

	const std::vector<std::string>& GetWords() const { return m_Words; }

	std::vector<std::string>& GetIncludePaths() { return m_IncludePaths; }
	const std::vector<std::string>& GetIncludePaths() const { return m_IncludePaths; }

	std::vector<std::string>& GetExtensions() { return m_Extensions; }

	const std::vector<std::string>& GetExtensions() const { return m_Extensions; }

	const std::string& GetFallbackExtension() const { return m_szFallbackExtension; }
	void SetFallbackExtension( const std::string& szFallbackExtension ) { m_szFallbackExtension = szFallbackExtension; }

	static std::shared_ptr<CConfiguration> Load( std::shared_ptr<IConfigurationManager> manager, const std::string& szFilename );

	bool Save();

	//Sets the name and saves it
	void Rename( const std::string& szNewName );

private:
	std::shared_ptr<IConfigurationManager> m_Manager;	//Manager used by this configuration
	std::string m_szName;								//Name of this configuration, without extension or path
	std::string m_szConfigFilename;						//Complete path and filename of a configuration file used to initialize the Angelscript API
	std::string m_szIncludeFilename;					//Name of a file to include in all files being compiled
	std::vector<std::string> m_Words;					//Preprocessor words to define
	std::vector<std::string> m_IncludePaths;			//Include paths to consider when searching for #include'd files.
														//Paths starting with '.' are considered to be relative paths

	std::vector<std::string> m_Extensions;				//List of extensions to consider when opening or saving files
	std::string m_szFallbackExtension;					//Extension to add when searching for #include'd files with no extension given
};

#endif // CCONFIGURATION_H
