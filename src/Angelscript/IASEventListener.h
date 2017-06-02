#ifndef ANGELSCRIPT_IASEVENTLISTENER_H
#define ANGELSCRIPT_IASEVENTLISTENER_H

#include <string>

struct ASCreationResult
{
	const std::string& szVersion;
	const bool fHasConfig;
};

struct ASAPIRegistrationResult
{
	const std::string& szConfigFilename;
	const bool fSuccess;
};

enum class ASEvent
{
	/**
	*	Parameter: const ASCreationResult*; Angelscript version as a string, and whether a configuration exists
	*/
	CREATED,		

	DESTROYED,

	/**
	*	Parameter: const ASAPIRegistrationResult*; an instance of an ASAPIRegistrationResult struct containing API registration results
	*/
	API_REGISTERED,

	/**
	*	Parameter: std::shared_ptr<const CScript>; the script currently being compiled
	*/
	COMPILATION_STARTED,

	/**
	*	Parameter: const bool*; whether compliation succeeded or failed
	*/
	COMPILATION_ENDED,

	/**
	*	Parameter: const std::string*; name of the configuration
	*/
	CONFIG_SET,

	/**
	*	Parameter: const std::string*; name of the configuration
	*/
	CONFIG_NOT_FOUND,

	CONFIG_CLEARED,
};

/*
 * Classes implementing this interface can listen to Angelscript events.
 */
class IASEventListener
{
public:
	virtual ~IASEventListener() = 0;

	virtual void AngelscriptEventOccured( ASEvent event, const void* pArg ) = 0;
};

inline IASEventListener::~IASEventListener()
{

}

#endif //ANGELSCRIPT_IASEVENTLISTENER_H
