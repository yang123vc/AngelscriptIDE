#ifndef ANGELSCRIPT_SCRIPTAPI_ASTYPES_H
#define ANGELSCRIPT_SCRIPTAPI_ASTYPES_H

class asIScriptEngine;

/**
*	Registers basic types used by Angelscript
*	@throws std::runtime_error If the size of asPWORD couldn't be deduced
*/
void RegisterASTypes( asIScriptEngine& scriptEngine );

#endif //ANGELSCRIPT_SCRIPTAPI_ASTYPES_H
