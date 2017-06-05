#ifndef ANGELSCRIPT_SCRIPTAPI_ASICONFIGURATION_H
#define ANGELSCRIPT_SCRIPTAPI_ASICONFIGURATION_H

#include <AngelscriptUtils/util/CASObjPtr.h>

class asIScriptContext;
class asIScriptEngine;
class asIScriptModule;
class asITypeInfo;
class CScriptBuilder;

/**
*	Registers the IConfiguration interface
*/
void RegisterIConfiguration( asIScriptEngine& scriptEngine );

/**
*	Given a module and the builder containing its metadata, finds the first implementation of the IConfiguration interface, instantiates it and returns it.
*/
CASObjPtr CreateIConfiguration( asIScriptModule& scriptModule, CScriptBuilder& scriptBuilder, asIScriptContext& scriptContext );

#endif //ANGELSCRIPT_SCRIPTAPI_ASICONFIGURATION_H
