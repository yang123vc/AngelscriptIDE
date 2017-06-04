#include <algorithm>

#include <QSettings.h>

#include "CConfigurationManager.h"
#include "COptions.h"

COptions::COptions()
	: m_ConfigurationManager( std::make_shared<CConfigurationManager>() )
{
}

COptions::~COptions()
{
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

void COptions::LoadOptions( QSettings& settings )
{
	ClearRecentFiles();

	settings.beginGroup( "general" );

	SetTabWidth( settings.value( "tabwidth", DEFAULT_TAB_WIDTH ).toInt() );
	SetStartMaximized( settings.value( "startmaximized", DEFAULT_START_MAXIMIZED ).toBool() );
	SetCurrentDirectory( settings.value( "currentdir" ).toString().toStdString() );

	const auto iRecentFilesCount = settings.beginReadArray( "recentfiles" );
	
	for( int i = 0; i < iRecentFilesCount; ++i )
	{
		settings.setArrayIndex( i );

		m_RecentFiles.emplace_back( settings.value( "file" ).toString().toStdString() );
	}

	settings.endArray();

	settings.endGroup();

	m_ConfigurationManager->LoadConfigurations( settings );
}

void COptions::SaveOptions( QSettings& settings )
{
	settings.beginGroup( "general" );

	settings.setValue( "tabwidth", GetTabWidth() );
	settings.setValue( "startmaximized", StartMaximized() );
	settings.setValue( "currentdir", GetCurrentDirectory().c_str() );

	settings.beginWriteArray( "recentfiles", m_RecentFiles.size() );

	int iFile = 0;

	for( const auto& file : m_RecentFiles )
	{
		settings.setArrayIndex( iFile++ );

		settings.setValue( "file", file.c_str() );
	}

	settings.endArray();

	settings.endGroup();

	m_ConfigurationManager->SaveConfigurations( settings );
}
