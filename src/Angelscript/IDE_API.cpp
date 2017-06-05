#include "IDE_API.h"
#include "ScriptAPI/ASIConfiguration.h"
#include "ScriptAPI/ASScriptEngineAPI.h"
#include "ScriptAPI/ASTypes.h"

void RegisterIDE_API( asIScriptEngine& scriptEngine )
{
	RegisterASTypes( scriptEngine );
	RegisterScriptEngineAPI( scriptEngine );

	RegisterIConfiguration( scriptEngine );
}
