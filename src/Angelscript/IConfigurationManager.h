#ifndef ANGELSCRIPT_ICONFIGURATIONMANAGER_H
#define ANGELSCRIPT_ICONFIGURATIONMANAGER_H

#include <memory>

class CConfiguration;
class IConfigurationEventListener;
class QSettings;

/**
*	Interface to the configuration manager
*/
class IConfigurationManager
{
public:
	/**
	*	Invalid configuration index
	*/
	static const size_t INVALID_INDEX = static_cast<size_t>( -1 );

public:

	virtual ~IConfigurationManager() = 0;

	virtual void AddConfigurationEventListener( IConfigurationEventListener* pListener ) = 0;

	virtual void RemoveConfigurationEventListener( IConfigurationEventListener* pListener ) = 0;

	virtual size_t GetConfigurationCount() const = 0;

	virtual std::shared_ptr<const CConfiguration> GetConfiguration( size_t uiIndex ) const = 0;
	virtual std::shared_ptr<CConfiguration> GetConfiguration( size_t uiIndex ) = 0;

	virtual size_t IndexOf( const std::shared_ptr<const CConfiguration>& config ) const = 0;

	virtual std::shared_ptr<const CConfiguration> Find( const std::string& szName ) const = 0;
	virtual std::shared_ptr<CConfiguration> Find( const std::string& szName ) = 0;

	virtual bool AddConfiguration( const std::shared_ptr<CConfiguration>& config ) = 0;

	virtual void RemoveConfiguration( const std::shared_ptr<CConfiguration>& config ) = 0;

	virtual std::shared_ptr<const CConfiguration> GetActiveConfiguration() const = 0;
	virtual std::shared_ptr<CConfiguration> GetActiveConfiguration() = 0;

	virtual void SetActiveConfiguration( const std::shared_ptr<const CConfiguration>& config ) = 0;
	virtual void SetActiveConfiguration( const std::string& szName ) = 0;

	virtual bool RenameConfiguration( const std::shared_ptr<CConfiguration>& config, const std::string& szNewName ) = 0;

	virtual void LoadConfigurations( QSettings& settings ) = 0;
	virtual void SaveConfigurations( QSettings& settings ) = 0;
};

inline IConfigurationManager::~IConfigurationManager()
{
}

#endif //ANGELSCRIPT_ICONFIGURATIONMANAGER_H
