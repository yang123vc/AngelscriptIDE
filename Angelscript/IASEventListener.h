#ifndef IASEVENTLISTENER_H
#define IASEVENTLISTENER_H

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
	Created,				//Parameter: const ASCreationResult*; Angelscript version as a string, and whether a configuration exists
	Destroyed,
	APIRegistered,			//Parameter: const ASAPIRegistrationResult*; an instance of an ASAPIRegistrationResult struct containing API registration results
	CompilationStarted,		//Parameter: std::shared_ptr<const CScript>; the script currently being compiled
	CompilationEnded,		//Parameter: const bool*; whether compliation succeeded or failed
	ConfigurationSet,		//Parameter: const std::string*; name of the configuration
	ConfigurationNotFound,	//Parameter: const std::string*; name of the configuration
	ConfigurationCleared,
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

#endif // IASEVENTLISTENER_H

