#ifndef IDE_CCONFIGURATIONMANAGER_H
#define IDE_CCONFIGURATIONMANAGER_H

#include <memory>
#include <vector>

#include "Angelscript/IConfigurationManager.h"
#include "util/CListenerManager.h"

class CASIDEApp;
class CConfiguration;
struct ConfigEvent;

/**
*	The configuration manager
*/
class CConfigurationManager : public std::enable_shared_from_this<CConfigurationManager>, public IConfigurationManager
{
public:
	CConfigurationManager();
	~CConfigurationManager() = default;

	void AddConfigurationEventListener( IConfigurationEventListener* pListener ) override;

	void RemoveConfigurationEventListener( IConfigurationEventListener* pListener ) override;

	size_t GetConfigurationCount() const override;

	std::shared_ptr<const CConfiguration> GetConfiguration( size_t uiIndex ) const override;
	std::shared_ptr<CConfiguration> GetConfiguration( size_t uiIndex ) override;

	size_t IndexOf( const std::shared_ptr<const CConfiguration>& config ) const override;

	std::shared_ptr<const CConfiguration> Find( const std::string& szName ) const override;
	std::shared_ptr<CConfiguration> Find( const std::string& szName ) override;

	bool AddConfiguration( const std::shared_ptr<CConfiguration>& config ) override;

	void RemoveConfiguration( const std::shared_ptr<CConfiguration>& config ) override;

	std::shared_ptr<const CConfiguration> GetActiveConfiguration() const override;
	std::shared_ptr<CConfiguration> GetActiveConfiguration() override;

	void SetActiveConfiguration( const std::shared_ptr<const CConfiguration>& config ) override;
	void SetActiveConfiguration( const std::string& szName ) override;

	bool RenameConfiguration( const std::shared_ptr<CConfiguration>& config, const std::string& szNewName ) override;

	void LoadConfigurations( QSettings& settings ) override;
	void SaveConfigurations( QSettings& settings ) override;

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

#endif //IDE_CCONFIGURATIONMANAGER_H
