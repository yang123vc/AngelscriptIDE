#ifndef ANGELSCRIPT_CCONFIGURATIONMANAGER_H
#define ANGELSCRIPT_CCONFIGURATIONMANAGER_H

#include <memory>
#include <vector>

#include "util/CListenerManager.h"

class CASIDEApp;
class CConfiguration;
struct ConfigEvent;
class IConfigurationEventListener;
class QSettings;

/**
*	The configuration manager
*/
class CConfigurationManager : public std::enable_shared_from_this<CConfigurationManager>
{
public:
	static const size_t INVALID_INDEX = static_cast<size_t>( -1 );

public:
	CConfigurationManager();
	~CConfigurationManager() = default;

	void AddConfigurationEventListener( IConfigurationEventListener* pListener );

	void RemoveConfigurationEventListener( IConfigurationEventListener* pListener );

	size_t GetConfigurationCount() const;

	std::shared_ptr<const CConfiguration> GetConfiguration( size_t uiIndex ) const;
	std::shared_ptr<CConfiguration> GetConfiguration( size_t uiIndex );

	size_t IndexOf( const std::shared_ptr<const CConfiguration>& config ) const;

	std::shared_ptr<const CConfiguration> Find( const std::string& szName ) const;
	std::shared_ptr<CConfiguration> Find( const std::string& szName );

	bool AddConfiguration( const std::shared_ptr<CConfiguration>& config );

	void RemoveConfiguration( const std::shared_ptr<CConfiguration>& config );

	std::shared_ptr<const CConfiguration> GetActiveConfiguration() const;
	std::shared_ptr<CConfiguration> GetActiveConfiguration();

	void SetActiveConfiguration( const std::shared_ptr<const CConfiguration>& config );
	void SetActiveConfiguration( const std::string& szName );

	bool RenameConfiguration( const std::shared_ptr<CConfiguration>& config, const std::string& szNewName );

	void LoadConfigurations( QSettings& settings );
	void SaveConfigurations( QSettings& settings );

protected:
	void NotifyListeners( const ConfigEvent& event );

private:
	CListenerManager<IConfigurationEventListener> m_ConfigurationListeners;

	std::vector<std::shared_ptr<CConfiguration>> m_Configurations;

	size_t m_uiActiveConfiguration = INVALID_INDEX;

private:
	CConfigurationManager( const CConfigurationManager& ) = delete;
	CConfigurationManager& operator=( const CConfigurationManager& ) = delete;
};

#endif //ANGELSCRIPT_CCONFIGURATIONMANAGER_H
