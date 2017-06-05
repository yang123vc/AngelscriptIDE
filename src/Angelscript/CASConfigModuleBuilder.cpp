#include <cassert>

#include <angelscript.h>

#include <AngelscriptUtils/CASModule.h>
#include <AngelscriptUtils/add_on/scriptbuilder.h>

#include "CASConfigModuleBuilder.h"
#include "CConfiguration.h"
#include "ScriptAPI/ASIConfiguration.h"

CASConfigModuleBuilder::CASConfigModuleBuilder( const std::shared_ptr<CConfiguration>& config, const CASRefPtr<asIScriptContext>& context )
	: m_Configuration( config )
	, m_Context( context )
{
	assert( config );
	assert( context );
}

bool CASConfigModuleBuilder::DefineWords( CScriptBuilder& builder )
{
	builder.DefineWord( "IDE_CONFIG_SCRIPT" );

	return true;
}

bool CASConfigModuleBuilder::AddScripts( CScriptBuilder& builder )
{
	return builder.AddSectionFromFile( m_Configuration->GetConfigScriptFilename().c_str() ) >= 0;
}

bool CASConfigModuleBuilder::IncludeScript( CScriptBuilder& builder,
					const char* const pszIncludeFileName,
					const char* const pszFromFileName )
{
	//Only support relative includes (relative to working dir)
	return builder.AddSectionFromFile( pszIncludeFileName ) >= 0;
}

bool CASConfigModuleBuilder::PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule )
{
	if( bSuccess )
	{
		//Find the configuration implementation
		m_ConfigurationObject = CreateIConfiguration( *pModule->GetModule(), builder, *m_Context );
	}

	return true;
}
