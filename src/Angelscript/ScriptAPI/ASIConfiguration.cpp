#include <cassert>

#include <angelscript.h>

#include <AngelscriptUtils/add_on/scriptbuilder.h>

#include "ASIConfiguration.h"

void RegisterIConfiguration( asIScriptEngine& scriptEngine )
{
	const char* const pszObjectName = "IConfiguration";

	auto result = scriptEngine.RegisterInterface( pszObjectName );
	assert( result >= 0 );

	result = scriptEngine.RegisterInterfaceMethod( pszObjectName, "void ConfigureEngine(asIScriptEngine@ pScriptEngine)" );
	assert( result >= 0 );
}

CASObjPtr CreateIConfiguration( asIScriptModule& scriptModule, CScriptBuilder& scriptBuilder, asIScriptContext& scriptContext )
{
	//TODO: could cache this - Solokiller
	//TODO: refactor this function to be an object that can find implementations of an interface, then cache as member - Solokiller
	auto pInterface = scriptModule.GetEngine()->GetTypeInfoByDecl( "interface IConfiguration" );

	asITypeInfo* pType = nullptr;

	for( decltype( scriptModule.GetObjectTypeCount() ) i = 0; i < scriptModule.GetObjectTypeCount(); ++i )
	{
		pType = scriptModule.GetObjectTypeByIndex( i );

		if( pType->DerivesFrom( pInterface ) )
		{
			auto pszMetaData = scriptBuilder.GetMetadataStringForType( pType->GetTypeId() );

			//If marked with "Configuration", it's the one we need
			if( pszMetaData && !strcmp( pszMetaData, "Configuration" ) )
			{
				break;
			}

			pType = nullptr;
		}
	}

	if( !pType )
		return {};

	//Find a default constructor
	asIScriptFunction* pFactory = nullptr;

	for( decltype( pType->GetFactoryCount() ) i = 0; i < pType->GetFactoryCount(); ++i )
	{
		pFactory = pType->GetFactoryByIndex( i );

		if( pFactory->GetParamCount() == 0 )
			break;

		pFactory = nullptr;
	}

	if( !pFactory )
		return {};

	auto result = scriptContext.Prepare( pFactory );
	assert( result >= 0 );

	result = scriptContext.Execute();
	assert( result >= 0 );

	auto pInstance = scriptContext.GetReturnObject();

	return { pInstance, pType };
}
