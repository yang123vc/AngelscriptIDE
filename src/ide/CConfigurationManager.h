#ifndef IDE_CCONFIGURATIONMANAGER_H
#define IDE_CCONFIGURATIONMANAGER_H

#include <memory>

#include "util/CListenerManager.h"

#include "Angelscript/IConfigurationManager.h"

class IConfigurationEventListener;
class CASIDEApp;
class CConfiguration;

class CConfigurationManager : public std::enable_shared_from_this<CConfigurationManager>, public IConfigurationManager
{
protected:
	friend class CConfiguration;

public:
	CConfigurationManager( std::shared_ptr<CASIDEApp> app );
	~CConfigurationManager();

	void AddConfigurationEventListener( IConfigurationEventListener* pListener ) override;

	void RemoveConfigurationEventListener( IConfigurationEventListener* pListener ) override;

	bool ConfigurationExists( const std::string& szName ) override;

	bool AddConfiguration( const std::string& szName ) override;

	void RemoveConfiguration( const std::string& szName, bool fRemoveFile = true ) override;

protected:
	void ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName ) override;

	void ConfigurationSaved( const std::string& szName ) override;

private:
	std::shared_ptr<CASIDEApp> m_App;
	CListenerManager<IConfigurationEventListener> m_ConfigurationListeners;

private:
	CConfigurationManager( const CConfigurationManager& ) = delete;
	CConfigurationManager& operator=( const CConfigurationManager& ) = delete;
};

#endif //IDE_CCONFIGURATIONMANAGER_H
