#ifndef ANGELSCRIPT_IASEVENTLISTENER_H
#define ANGELSCRIPT_IASEVENTLISTENER_H

#include <memory>
#include <string>

class CScript;

struct ASCreationEvent
{
	/**
	*	The Angelscript version as a string
	*/
	const std::string* pszVersion;

	/**
	*	Whether a configuration exists for this instance
	*/
	bool bHasConfig;
};

struct ASAPIRegistrationEvent
{
	/**
	*	Name of the config file that was loaded
	*/
	const std::string* pszConfigFilename;

	/**
	*	Whether registration succeded
	*/
	bool bSuccess;
};

struct ASCompilationStartEvent
{
	/**
	*	The script currently being compiled
	*/
	const CScript* pScript;
};

struct ASCompilationEndEvent
{
	/**
	*	The script currently being compiled
	*/
	const CScript* pScript;

	/**
	*	Whether compilation succeeded or failed
	*/
	bool bSuccess;
};

enum class ASConfigChangeType
{
	SET = 0,
	CLEARED
};

struct ASConfigChangeEvent
{
	ASConfigChangeType changeType;
	const std::string* pszName;
};

enum class ASEventType
{
	INVALID = 0,

	CREATED,		
	DESTROYED,

	API_REGISTERED,

	COMPILATION_STARTED,
	COMPILATION_ENDED,

	CONFIG_CHANGE,
};

struct ASEvent
{
	ASEvent( ASEventType type )
		: type( type )
	{
	}

	const ASEventType type;

	union
	{
		ASCreationEvent create;
		ASAPIRegistrationEvent apiRegistration;
		ASCompilationStartEvent compilationStart;
		ASCompilationEndEvent compilationEnd;
		ASConfigChangeEvent configChange;
	};
};

/*
 * Classes implementing this interface can listen to Angelscript events.
 */
class IASEventListener
{
public:
	virtual ~IASEventListener() = 0;

	virtual void AngelscriptEventOccured( const ASEvent& event ) = 0;
};

inline IASEventListener::~IASEventListener()
{

}

#endif //ANGELSCRIPT_IASEVENTLISTENER_H
