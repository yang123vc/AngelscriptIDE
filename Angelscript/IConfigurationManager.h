#ifndef ICONFIGURATIONMANAGER_H
#define ICONFIGURATIONMANAGER_H

class IConfigurationEventListener;

class IConfigurationManager
{
protected:
	friend class CConfiguration;

public:

	virtual ~IConfigurationManager() = 0;

	virtual void AddConfigurationEventListener( IConfigurationEventListener* pListener ) = 0;

	virtual void RemoveConfigurationEventListener( IConfigurationEventListener* pListener ) = 0;

	virtual bool ConfigurationExists( const std::string& szName ) = 0;

	virtual bool AddConfiguration( const std::string& szName ) = 0;

	virtual void RemoveConfiguration( const std::string& szName, bool fRemoveFile = true ) = 0;

protected:
	virtual void ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName ) = 0;

	virtual void ConfigurationSaved( const std::string& szName ) = 0;
};

inline IConfigurationManager::~IConfigurationManager()
{
}

#endif // ICONFIGURATIONMANAGER_H

