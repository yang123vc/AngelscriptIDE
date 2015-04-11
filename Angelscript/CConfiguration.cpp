#include <cstdio>

#include "util/CProperties.h"
#include "util/StringUtils.h"

#include "ConfigurationException.h"
#include "IConfigurationManager.h"

#include "CConfiguration.h"

const char* CConfiguration::CONFIG_DIRECTORY = "configs";
const char* CConfiguration::CONFIG_FILE_TYPE = ".config";

CConfiguration::CConfiguration( std::shared_ptr<IConfigurationManager> manager, const std::string& szName, const std::string& szConfigFilename )
	: m_Manager( manager )
{
	SetName( szName );
	SetConfigFilename( szConfigFilename );
}

std::string CConfiguration::MakeConfigurationPath( const std::string& szName )
{
	return std::string( "./" ) + CONFIG_DIRECTORY + '/' + szName + CONFIG_FILE_TYPE;
}

void CConfiguration::SetName( const std::string& szName )
{
	m_szName = szName;
}

void CConfiguration::SetConfigFilename( const std::string& szConfigFilename )
{
	m_szConfigFilename = szConfigFilename;
}

void CConfiguration::SetIncludeFilename( const std::string& szIncludeFilename )
{
	m_szIncludeFilename = szIncludeFilename;
}

std::shared_ptr<CConfiguration> CConfiguration::Load( std::shared_ptr<IConfigurationManager> manager, const std::string& szFilename )
{
	CProperties properties;

	if( !properties.Load( MakeConfigurationPath( szFilename ) ) )
		return std::shared_ptr<CConfiguration>();

	const auto& keyvalues = properties.GetKeyvalues();

	auto asconfig = keyvalues.find( "asconfig" );

	auto includeFilename = keyvalues.find( "includefilename" );

	auto words = keyvalues.find( "words" );

	auto includePaths = keyvalues.find( "includepaths" );

	auto extensions = keyvalues.find( "extensions" );

	auto fallbackExtension = keyvalues.find( "fallbackextensions" );

	auto config = std::make_shared<CConfiguration>(
				manager,
				szFilename,
				asconfig != keyvalues.end() ? asconfig->second : "" );

	config->SetIncludeFilename( includeFilename != keyvalues.end() ? includeFilename->second : "" );

	if( words != keyvalues.end() )
	{
		auto& wordsVec = config->GetWords();

		Split( words->second, WORD_DELIMITER, wordsVec );
	}

	if( includePaths != keyvalues.end() )
	{
		auto& includePathsVec = config->GetIncludePaths();

		Split( includePaths->second, INCLUDEPATH_DELIMITER, includePathsVec );
	}

	if( extensions != keyvalues.end() )
	{
		auto& extensionsVec = config->GetExtensions();

		Split( extensions->second, EXTENSION_DELIMITER, extensionsVec );
	}

	if( fallbackExtension != keyvalues.end() )
		config->SetFallbackExtension( fallbackExtension->second );

	return config;
}

bool CConfiguration::Save()
{
	CProperties properties;

	auto& keyvalues = properties.GetKeyvalues();

	keyvalues.insert( { "asconfig", m_szConfigFilename } );
	keyvalues.insert( { "includefilename", m_szIncludeFilename } );

	keyvalues.insert( { "words", Implode( GetWords(), WORD_DELIMITER ) } );

	keyvalues.insert( { "includepaths", Implode( GetIncludePaths(), INCLUDEPATH_DELIMITER ) } );

	keyvalues.insert( { "extensions", Implode( GetExtensions(), EXTENSION_DELIMITER ) } );

	keyvalues.insert( { "fallbackextensions", m_szFallbackExtension } );

	bool fResult = properties.Save( MakeConfigurationPath( m_szName ) );

	if( fResult )
		m_Manager->ConfigurationSaved( GetName() );

	return fResult;
}

void CConfiguration::Rename( const std::string& szNewName )
{
	if( m_Manager->ConfigurationExists( szNewName ) )
		throw ConfigurationException( "Attempted to rename configuration to an existing configuration!" );

	const std::string szOldName = std::move( m_szName );

	SetName( szNewName );

	if( Save() )
	{
		m_Manager->ConfigurationRenamed( szOldName, szNewName );

		remove( ( szOldName + CONFIG_FILE_TYPE ).c_str() );
	}
}
