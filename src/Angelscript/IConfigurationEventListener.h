#ifndef ICONFIGURATIONEVENTLISTENER_H
#define ICONFIGURATIONEVENTLISTENER_H

#include <string>

class IConfigurationEventListener
{
public:
	virtual ~IConfigurationEventListener() = 0;

	virtual void ConfigurationAdded( const std::string& szName );

	virtual void ConfigurationRemoved( const std::string& szName );

	virtual void ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName );

	virtual void ConfigurationSaved( const std::string& szName );
};

inline IConfigurationEventListener::~IConfigurationEventListener()
{
}

inline void IConfigurationEventListener::ConfigurationAdded( const std::string& )
{
}

inline void IConfigurationEventListener::ConfigurationRemoved( const std::string& )
{
}

inline void IConfigurationEventListener::ConfigurationRenamed( const std::string&, const std::string& )
{
}

inline void IConfigurationEventListener::ConfigurationSaved( const std::string& )
{
}

#endif // ICONFIGURATIONEVENTLISTENER_H

