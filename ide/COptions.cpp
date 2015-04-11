#include <algorithm>
#include <iostream>

#include "util/CProperties.h"
#include "util/StringUtils.h"

#include "COptions.h"

COptions::COptions( const std::string& szFilename )
	: m_szFilename( szFilename )
	, m_iTabWidth( DEFAULT_TAB_WIDTH )
	, m_fMaximized( true )
{

}

void COptions::SetActiveConfigurationName( const std::string& szActiveConfiguration )
{
	m_szActiveConfiguration = szActiveConfiguration;
}

void COptions::AddRecentFile( const std::string& szFilename )
{
	auto it = std::find( m_RecentFiles.begin(), m_RecentFiles.end(), szFilename );

	//If it's already in the list, remove it first.
	if( it != m_RecentFiles.end() )
		m_RecentFiles.erase( it );

	//Make latest file accessed
	m_RecentFiles.push_front( szFilename );

	//Remove oldest entries
	if( m_RecentFiles.size() > MAX_RECENT_FILES )
		m_RecentFiles.resize( MAX_RECENT_FILES );
}

void COptions::RemoveRecentFile( const std::string& szFilename )
{
	auto it = std::find( m_RecentFiles.begin(), m_RecentFiles.end(), szFilename );

	if( it != m_RecentFiles.end() )
		m_RecentFiles.erase( it );
}

void COptions::ClearRecentFiles()
{
	m_RecentFiles.clear();
}

std::shared_ptr<COptions> COptions::Load( const std::string& szFilename, bool* pLoaded )
{
	CProperties properties;

	auto options = std::make_shared<COptions>( szFilename );

	bool fLoaded = false;

	if( properties.Load( szFilename ) )
	{
		const auto& keyvalues = properties.GetKeyvalues();

		auto configs = keyvalues.find( "configs" );

		if( configs != keyvalues.end() )
			Split( configs->second, CONFIGS_DELIMITER, options->GetConfigurations() );

		auto activeConfig = keyvalues.find( "activeconfig" );

		options->m_szActiveConfiguration = activeConfig != keyvalues.end() ? activeConfig->second : "";

		auto tabWidth = keyvalues.find( "tabwidth" );

		options->m_iTabWidth = tabWidth != keyvalues.end() ? std::stol( tabWidth->second ) : DEFAULT_TAB_WIDTH;

		auto startMaximized = keyvalues.find( "startmaximized" );

		options->m_fMaximized = startMaximized != keyvalues.end() ? startMaximized->second == "true" : true;

		auto recentFiles = keyvalues.find( "recentfiles" );

		if( recentFiles != keyvalues.end() )
		{
			Split( recentFiles->second, RECENTFILES_DELIMITER, options->m_RecentFiles );

			if( options->m_RecentFiles.size() > MAX_RECENT_FILES )
				options->m_RecentFiles.resize( MAX_RECENT_FILES );
		}

		auto currentDir = keyvalues.find( "currentdir" );

		options->SetCurrentDirectory( currentDir != keyvalues.end() ? currentDir->second : "." );

		fLoaded = true;
	}

	if( pLoaded )
		*pLoaded = fLoaded;

	return options;
}

bool COptions::Save()
{
	CProperties properties;

	auto& keyvalues = properties.GetKeyvalues();

	keyvalues.insert( { "configs", Implode( GetConfigurations(), CONFIGS_DELIMITER ) } );

	keyvalues.insert( { "activeconfig", m_szActiveConfiguration } );

	keyvalues.insert( { "tabwidth", std::to_string( m_iTabWidth ) } );

	keyvalues.insert( { "startmaximized", m_fMaximized ? "true" : "false" } );

	keyvalues.insert( { "recentfiles", Implode( GetRecentFiles(), RECENTFILES_DELIMITER ) } );

	keyvalues.insert( { "currentdir", m_szCurrentDir } );

	return properties.Save( m_szFilename );
}
