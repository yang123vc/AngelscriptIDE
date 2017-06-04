#ifndef ANGELSCRIPT_ICONFIGURATIONEVENTLISTENER_H
#define ANGELSCRIPT_ICONFIGURATIONEVENTLISTENER_H

#include <memory>
#include <string>

class CConfiguration;

struct ConfigAddEvent
{
	const std::string* pszName;
};

struct ConfigRemoveEvent
{
	const std::string* pszName;
	bool bIsActiveConfig;
};

struct ConfigRenameEvent
{
	const std::string* pszOldName;
	const std::string* pszNewName;
	bool bIsActiveConfig;
};

struct ConfigChangeEvent
{
	const CConfiguration* pOldConfig;
	const CConfiguration* pNewConfig;
};

enum class ConfigEventType
{
	INVALID = 0,

	ADD,
	REMOVE,
	RENAME,
	CHANGE,
};

struct ConfigEvent
{
	ConfigEvent( ConfigEventType type )
		: type( type )
	{
	}

	const ConfigEventType type;

	union
	{
		ConfigAddEvent add;
		ConfigRemoveEvent remove;
		ConfigRenameEvent rename;
		ConfigChangeEvent change;
	};
};

class IConfigurationEventListener
{
public:
	virtual ~IConfigurationEventListener() = 0;

	virtual void ConfigEventOccurred( const ConfigEvent& event ) = 0;
};

inline IConfigurationEventListener::~IConfigurationEventListener()
{
}

#endif //ANGELSCRIPT_ICONFIGURATIONEVENTLISTENER_H
