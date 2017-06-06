#include <algorithm>

#include <QColor>
#include <QFont>
#include <QSettings>

#include "COptions.h"
#include "Angelscript/CConfigurationManager.h"

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

void COptions::SetPatterns( QVector<Pattern>&& patterns )
{
	m_Patterns = std::move( patterns );
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

	LoadPatterns( settings, m_Patterns );

	m_ConfigurationManager->LoadConfigurations( settings );

	OptionsLoaded();
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

	settings.beginGroup( "patterns" );

	for( const auto& pattern : m_Patterns )
	{
		settings.beginGroup( pattern.m_szPatternName );

		settings.setValue( "pattern", pattern.m_szPattern );
		settings.setValue( "fg_color", pattern.m_FGColor.color() );
		settings.setValue( "bg_color", pattern.m_BGColor.color() );
		settings.setValue( "bold", pattern.m_bBold );
		settings.setValue( "underline", pattern.m_bUnderline );

		settings.endGroup();
	}

	settings.endGroup();

	m_ConfigurationManager->SaveConfigurations( settings );

	OptionsSaved();
}

void COptions::LoadPatterns( QSettings& settings, QVector<Pattern>& patterns )
{
	patterns.clear();

	settings.beginGroup( "patterns" );

	for( const auto& szPatternName : settings.childGroups() )
	{
		settings.beginGroup( szPatternName );

		auto szPattern = settings.value( "pattern" ).toString();

		szPattern = szPattern.trimmed();

		if( !szPattern.isEmpty() )
		{
			Pattern pattern;

			pattern.m_szPatternName = szPatternName;
			pattern.m_szPattern = std::move( szPattern );
			pattern.m_FGColor = settings.value( "fg_color", QColor( Qt::black ) ).value<QColor>();
			pattern.m_BGColor = settings.value( "bg_color", QColor( Qt::black ) ).value<QColor>();
			pattern.m_bBold = settings.value( "bold", false ).toBool();
			pattern.m_bUnderline = settings.value( "underline", false ).toBool();

			patterns.push_back( std::move( pattern ) );
		}
		else
		{
			//TODO: log error - Solokiller
		}

		settings.endGroup();
	}

	settings.endGroup();
}
