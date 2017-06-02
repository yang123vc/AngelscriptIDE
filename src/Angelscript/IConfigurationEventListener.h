#ifndef ANGELSCRIPT_ICONFIGURATIONEVENTLISTENER_H
#define ANGELSCRIPT_ICONFIGURATIONEVENTLISTENER_H

#include <string>

struct ConfigAddEvent
{
	const std::string* pszName;
};

struct ConfigRemoveEvent
{
	const std::string* pszName;
};

struct ConfigRenameEvent
{
	const std::string* pszOldName;
	const std::string* pszNewName;
};

struct ConfigSaveEvent
{
	const std::string* pszName;
};

enum class ConfigEventType
{
	INVALID = 0,

	ADD,
	REMOVE,
	RENAME,
	SAVE
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
		ConfigSaveEvent save;
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
