#ifndef ANGELSCRIPT_IDE_API_H
#define ANGELSCRIPT_IDE_API_H

class asIScriptEngine;

/**
*	Registers the IDE API for the given engine
*	@throws std::runtime_error
*/
void RegisterIDE_API( asIScriptEngine& scriptEngine );

#endif //ANGELSCRIPT_IDE_API_H
