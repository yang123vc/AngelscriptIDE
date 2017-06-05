#include <cassert>

#include <angelscript.h>

#include "ASScriptEngineAPI.h"

static void RegisterasEEngineProp( asIScriptEngine& scriptEngine )
{
	const char* const pszObjectName = "asEEngineProp";

	auto result = scriptEngine.RegisterEnum( pszObjectName );
	assert( result >= 0 );

	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_ALLOW_UNSAFE_REFERENCES", asEP_ALLOW_UNSAFE_REFERENCES );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_OPTIMIZE_BYTECODE", asEP_OPTIMIZE_BYTECODE );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_COPY_SCRIPT_SECTIONS", asEP_COPY_SCRIPT_SECTIONS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_MAX_STACK_SIZE", asEP_MAX_STACK_SIZE );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_USE_CHARACTER_LITERALS", asEP_USE_CHARACTER_LITERALS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_ALLOW_MULTILINE_STRINGS", asEP_ALLOW_MULTILINE_STRINGS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_ALLOW_IMPLICIT_HANDLE_TYPES", asEP_ALLOW_IMPLICIT_HANDLE_TYPES );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_BUILD_WITHOUT_LINE_CUES", asEP_BUILD_WITHOUT_LINE_CUES );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_INIT_GLOBAL_VARS_AFTER_BUILD", asEP_INIT_GLOBAL_VARS_AFTER_BUILD );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_REQUIRE_ENUM_SCOPE", asEP_REQUIRE_ENUM_SCOPE );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_SCRIPT_SCANNER", asEP_SCRIPT_SCANNER );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_INCLUDE_JIT_INSTRUCTIONS", asEP_INCLUDE_JIT_INSTRUCTIONS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_STRING_ENCODING", asEP_STRING_ENCODING );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_PROPERTY_ACCESSOR_MODE", asEP_PROPERTY_ACCESSOR_MODE );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_EXPAND_DEF_ARRAY_TO_TMPL", asEP_EXPAND_DEF_ARRAY_TO_TMPL );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_AUTO_GARBAGE_COLLECT", asEP_AUTO_GARBAGE_COLLECT );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_DISALLOW_GLOBAL_VARS", asEP_DISALLOW_GLOBAL_VARS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_ALWAYS_IMPL_DEFAULT_CONSTRUCT", asEP_ALWAYS_IMPL_DEFAULT_CONSTRUCT );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_COMPILER_WARNINGS", asEP_COMPILER_WARNINGS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE", asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_ALTER_SYNTAX_NAMED_ARGS", asEP_ALTER_SYNTAX_NAMED_ARGS );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_DISABLE_INTEGER_DIVISION", asEP_DISABLE_INTEGER_DIVISION );
	assert( result >= 0 );
	result = scriptEngine.RegisterEnumValue( pszObjectName, "asEP_DISALLOW_EMPTY_LIST_ELEMENTS", asEP_DISALLOW_EMPTY_LIST_ELEMENTS );
	assert( result >= 0 );
}

void RegisterScriptEngineAPI( asIScriptEngine& scriptEngine )
{
	RegisterasEEngineProp( scriptEngine );

	const char* const pszObjectName = "asIScriptEngine";

	auto result = scriptEngine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );
	assert( result >= 0 );

	result = scriptEngine.RegisterObjectMethod(
		pszObjectName, "int SetEngineProperty(asEEngineProp property, asPWORD value)",
		asMETHOD( asIScriptEngine, SetEngineProperty ), asCALL_THISCALL );
	assert( result >= 0 );

	result = scriptEngine.RegisterObjectMethod(
		pszObjectName, "asPWORD GetEngineProperty(asEEngineProp property)",
		asMETHOD( asIScriptEngine, GetEngineProperty ), asCALL_THISCALL );
	assert( result >= 0 );
}
