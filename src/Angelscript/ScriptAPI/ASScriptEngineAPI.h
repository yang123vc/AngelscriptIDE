#ifndef ANGELSCRIPT_SCRIPTAPI_ASSCRIPTENGINEAPI_H
#define ANGELSCRIPT_SCRIPTAPI_ASSCRIPTENGINEAPI_H

class asIScriptEngine;

/**
*	Registers asIScriptEngine's API for use in scripts
*	@throws std::runtime_error
*/
void RegisterScriptEngineAPI( asIScriptEngine& scriptEngine );

#endif //ANGELSCRIPT_SCRIPTAPI_ASSCRIPTENGINEAPI_H
