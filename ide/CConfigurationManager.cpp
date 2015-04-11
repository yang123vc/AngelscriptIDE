#include "Angelscript/CConfiguration.h"
#include "Angelscript/IConfigurationEventListener.h"

#include "Angelscript/ConfigurationException.h"

#include "COptions.h"
#include "CASIDEApp.h"

#include "CConfigurationManager.h"

CConfigurationManager::CConfigurationManager( std::shared_ptr<CASIDEApp> app )
	: m_App( app )
{

}

CConfigurationManager::~CConfigurationManager()
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

bool CConfigurationManager::ConfigurationExists( const std::string& szName )
{
	auto config = CConfiguration::Load( shared_from_this(), szName );

	return config != nullptr;
}

bool CConfigurationManager::AddConfiguration( const std::string& szName )
{
	COptions::Configurations_t& configs = m_App->GetOptions()->GetConfigurations();

	auto it = std::find( configs.begin(), configs.end(), szName );

	if( it != configs.end() )
	{
		throw ConfigurationException( "Attempted to add duplicate configuration!" );
		return false;
	}

	auto config = CConfiguration::Load( shared_from_this(), szName );

	//If it doesn't already exist, create it
	if( !config )
		config = std::make_shared<CConfiguration>( shared_from_this(), szName );

	const bool fSuccess = config->Save();

	if( fSuccess )
	{
		configs.push_back( szName );

		m_ConfigurationListeners.NotifyListeners( &IConfigurationEventListener::ConfigurationAdded, szName );
	}
	else
		throw ConfigurationException( std::string( "Failed to save configuration \"" ) + szName + "\"!" );

	return fSuccess;
}

void CConfigurationManager::RemoveConfiguration( const std::string& szName, bool fRemoveFile )
{
	COptions::Configurations_t& configs = m_App->GetOptions()->GetConfigurations();

	auto it = std::find( configs.begin(), configs.end(), szName );

	if( it == configs.end() )
	{
		throw ConfigurationException( "Attempted to remove non-existent configuration!" );
		return;
	}

	configs.erase( it );

	if( fRemoveFile )
		remove( CConfiguration::MakeConfigurationPath( szName ).c_str() );

	m_ConfigurationListeners.NotifyListeners( &IConfigurationEventListener::ConfigurationRemoved, szName );
}

void CConfigurationManager::ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName )
{
	auto options = m_App->GetOptions();

	COptions::Configurations_t& configs = options->GetConfigurations();

	configs.push_back( szNewName );

	auto it = std::find( configs.begin(), configs.end(), szOldName );

	if( it == configs.end() )
	{
		throw ConfigurationException( "Attempted to rename non-existent configuration!" );
		return;
	}

	configs.erase( it );

	m_ConfigurationListeners.NotifyListeners( &IConfigurationEventListener::ConfigurationRenamed, szOldName, szNewName );
}

void CConfigurationManager::ConfigurationSaved( const std::string& szName )
{
	m_ConfigurationListeners.NotifyListeners( &IConfigurationEventListener::ConfigurationSaved, szName );
}
