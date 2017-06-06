#include <QSettings.h>

#include "Angelscript/CConfiguration.h"
#include "Angelscript/CConfigurationException.h"
#include "Angelscript/IConfigurationEventListener.h"
#include "CConfigurationManager.h"

CConfigurationManager::CConfigurationManager()
{
}

void CConfigurationManager::AddConfigurationEventListener( IConfigurationEventListener* pListener )
{
	m_ConfigurationListeners.AddListener( pListener );
}

void CConfigurationManager::RemoveConfigurationEventListener( IConfigurationEventListener* pListener )
{
	m_ConfigurationListeners.RemoveListener( pListener );
}

size_t CConfigurationManager::GetConfigurationCount() const
{
	return m_Configurations.size();
}

std::shared_ptr<const CConfiguration> CConfigurationManager::GetConfiguration( size_t uiIndex ) const
{
	return const_cast<CConfigurationManager*>( this )->GetConfiguration( uiIndex );
}

std::shared_ptr<CConfiguration> CConfigurationManager::GetConfiguration( size_t uiIndex )
{
	return m_Configurations[ uiIndex ];
}

size_t CConfigurationManager::IndexOf( const std::shared_ptr<const CConfiguration>& config ) const
{
	for( size_t uiIndex = 0; uiIndex < m_Configurations.size(); ++uiIndex )
	{
		if( m_Configurations[ uiIndex ] == config )
			return uiIndex;
	}

	return INVALID_INDEX;
}

std::shared_ptr<const CConfiguration> CConfigurationManager::Find( const std::string& szName ) const
{
	return const_cast<CConfigurationManager*>( this )->Find( szName );
}

std::shared_ptr<CConfiguration> CConfigurationManager::Find( const std::string& szName )
{
	for( size_t uiIndex = 0; uiIndex < m_Configurations.size(); ++uiIndex )
	{
		if( m_Configurations[ uiIndex ]->GetName() == szName )
			return m_Configurations[ uiIndex ];
	}

	return nullptr;
}

bool CConfigurationManager::AddConfiguration( const std::shared_ptr<CConfiguration>& config )
{
	if( !config )
		return false;

	if( config->GetName().empty() )
		throw CConfigurationException( "A configuration must have a valid name" );

	if( Find( config->GetName() ) )
		throw CConfigurationException( "Attempted to add duplicate configuration" );

	m_Configurations.emplace_back( config );

	ConfigEvent addEvent( ConfigEventType::ADD );

	addEvent.add.pszName = &config->GetName();

	NotifyListeners( addEvent );

	return true;
}

void CConfigurationManager::RemoveConfiguration( const std::shared_ptr<CConfiguration>& config )
{
	if( !config )
		throw CConfigurationException( "Attempted to remove null configuration" );

	const auto uiIndex = IndexOf( config );

	if( uiIndex == INVALID_INDEX )
	{
		throw CConfigurationException( "Attempted to remove non-existent configuration" );
	}

	const bool bIsActiveConfig = uiIndex == m_uiActiveConfiguration;

	if( bIsActiveConfig )
	{
		//TODO: switch to next available config - Solokiller
		SetActiveConfiguration( std::shared_ptr<CConfiguration>() );
	}

	m_Configurations.erase( m_Configurations.begin() + uiIndex );

	ConfigEvent removeEvent( ConfigEventType::REMOVE );

	removeEvent.remove.pszName = &config->GetName();
	removeEvent.remove.bIsActiveConfig = bIsActiveConfig;

	NotifyListeners( removeEvent );
}

std::shared_ptr<const CConfiguration> CConfigurationManager::GetActiveConfiguration() const
{
	return const_cast<CConfigurationManager*>( this )->GetActiveConfiguration();
}

std::shared_ptr<CConfiguration> CConfigurationManager::GetActiveConfiguration()
{
	return m_uiActiveConfiguration != INVALID_INDEX ? m_Configurations[ m_uiActiveConfiguration ] : nullptr;
}

void CConfigurationManager::SetActiveConfiguration( const std::shared_ptr<const CConfiguration>& config )
{
	auto oldConfig = GetActiveConfiguration();

	if( config )
	{
		m_uiActiveConfiguration = IndexOf( config );
	}
	else
	{
		m_uiActiveConfiguration = INVALID_INDEX;
	}

	ConfigEvent changeEvent( ConfigEventType::CHANGE );

	changeEvent.change.pOldConfig = oldConfig.get();
	changeEvent.change.pNewConfig = config.get();

	NotifyListeners( changeEvent );
}

void CConfigurationManager::SetActiveConfiguration( const std::string& szName )
{
	SetActiveConfiguration( Find( szName ) );
}

bool CConfigurationManager::RenameConfiguration( const std::shared_ptr<CConfiguration>& config, const std::string& szNewName )
{
	if( !config )
		throw CConfigurationException( "Attempted to rename null configuration" );

	if( config->GetName().empty() )
		throw CConfigurationException( "A configuration must have a valid name" );

	if( config->GetName() == szNewName )
		return true;

	if( Find( szNewName ) )
		return false;

	const auto szOldName = config->GetName();

	config->SetName( szNewName );

	ConfigEvent renameEvent( ConfigEventType::RENAME );

	renameEvent.rename.pszOldName = &szOldName;
	renameEvent.rename.pszNewName = &szNewName;
	renameEvent.rename.bIsActiveConfig = config == GetActiveConfiguration();

	NotifyListeners( renameEvent );

	return true;
}

void CConfigurationManager::LoadConfigurations( QSettings& settings )
{
	settings.beginGroup( "configurationmanager" );

	m_Configurations.clear();

	const auto iConfigsCount = settings.beginReadArray( "configs" );

	for( int i = 0; i < iConfigsCount; ++i )
	{
		settings.setArrayIndex( i );

		auto szName = settings.value( "name" ).toString().toStdString();

		if( !szName.empty() )
		{
			auto config = std::make_shared<CConfiguration>( std::move( szName ) );

			config->SetConfigFilename( settings.value( "asconfig" ).toString().toStdString() );
			config->SetConfigScriptFilename( settings.value( "configscript" ).toString().toStdString() );
			config->SetIncludeFilename( settings.value( "includefilename" ).toString().toStdString() );
			config->SetFallbackExtension( settings.value( "fallbackextension" ).toString().toStdString() );

			const auto iWordsCount = settings.beginReadArray( "words" );

			for( int iWord = 0; iWord < iWordsCount; ++iWord )
			{
				settings.setArrayIndex( iWord );

				config->GetWords().emplace_back( settings.value( "word" ).toString().toStdString() );
			}

			settings.endArray();

			const auto iPathsCount = settings.beginReadArray( "includepaths" );

			for( int iPath = 0; iPath < iPathsCount; ++iPath )
			{
				settings.setArrayIndex( iPath );

				config->GetIncludePaths().emplace_back( settings.value( "path" ).toString().toStdString() );
			}

			settings.endArray();

			const auto iExtsCount = settings.beginReadArray( "extensions" );

			for( int iExt = 0; iExt < iExtsCount; ++iExt )
			{
				settings.setArrayIndex( iExt );

				config->GetExtensions().emplace_back( settings.value( "ext" ).toString().toStdString() );
			}

			settings.endArray();

			AddConfiguration( config );
		}
	}

	settings.endArray();

	const auto szActiveConfiguration = settings.value( "activeconfiguration" ).toString().toStdString();

	//Any old config is wiped here
	SetActiveConfiguration( Find( szActiveConfiguration ) );

	settings.endGroup();
}

void CConfigurationManager::SaveConfigurations( QSettings& settings )
{
	settings.beginGroup( "configurationmanager" );

	settings.beginWriteArray( "configs", m_Configurations.size() );

	int iIndex = 0;

	for( const auto& config : m_Configurations )
	{
		settings.setArrayIndex( iIndex++ );

		settings.setValue( "name", QString( config->GetName().c_str() ) );
		settings.setValue( "asconfig", QString( config->GetConfigFilename().c_str() ) );
		settings.setValue( "configscript", QString( config->GetConfigScriptFilename().c_str() ) );
		settings.setValue( "includefilename", QString( config->GetIncludeFilename().c_str() ) );
		settings.setValue( "fallbackextension", QString( config->GetFallbackExtension().c_str() ) );

		const auto& words = config->GetWords();

		settings.beginWriteArray( "words", words.size() );

		for( size_t uiIndex = 0; uiIndex < words.size(); ++uiIndex )
		{
			settings.setArrayIndex( uiIndex );

			settings.setValue( "word", QString( words[ uiIndex ].c_str() ) );
		}

		settings.endArray();

		const auto& includePaths = config->GetIncludePaths();

		settings.beginWriteArray( "includepaths", includePaths.size() );

		for( size_t uiIndex = 0; uiIndex < includePaths.size(); ++uiIndex )
		{
			settings.setArrayIndex( uiIndex );

			settings.setValue( "path", QString( includePaths[ uiIndex ].c_str() ) );
		}

		settings.endArray();

		const auto& extensions = config->GetExtensions();

		settings.beginWriteArray( "extensions", extensions.size() );

		for( size_t uiIndex = 0; uiIndex < extensions.size(); ++uiIndex )
		{
			settings.setArrayIndex( uiIndex );

			settings.setValue( "ext", QString( extensions[ uiIndex ].c_str() ) );
		}

		settings.endArray();
	}

	settings.endArray();

	auto activeConfig = GetActiveConfiguration();

	settings.setValue( "activeconfiguration", QString( activeConfig ? activeConfig->GetName().c_str() : "" ) );

	settings.endGroup();
}

void CConfigurationManager::NotifyListeners( const ConfigEvent& event )
{
	m_ConfigurationListeners.NotifyListeners( &IConfigurationEventListener::ConfigEventOccurred, event );
}
