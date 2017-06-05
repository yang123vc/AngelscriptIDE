#ifndef ANGELSCRIPT_CASCONFIGMODULEBUILDER_H
#define ANGELSCRIPT_CASCONFIGMODULEBUILDER_H

#include <memory>

#include <AngelscriptUtils/IASModuleBuilder.h>
#include <AngelscriptUtils/util/CASObjPtr.h>
#include <AngelscriptUtils/util/CASRefPtr.h>

class asIScriptContext;
class CConfiguration;

class CASConfigModuleBuilder final : public IASModuleBuilder
{
public:
	CASConfigModuleBuilder( const std::shared_ptr<CConfiguration>& config, const CASRefPtr<asIScriptContext>& context );
	~CASConfigModuleBuilder() = default;

	bool DefineWords( CScriptBuilder& builder ) override;

	bool AddScripts( CScriptBuilder& builder ) override;

	bool IncludeScript( CScriptBuilder& builder,
						const char* const pszIncludeFileName,
						const char* const pszFromFileName ) override;

	bool PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule ) override;

	/**
	*	@return The configuration object provided by the script, if any
	*/
	const CASObjPtr& GetConfigurationObject() const { return m_ConfigurationObject; }

private:
	std::shared_ptr<CConfiguration> m_Configuration;
	CASRefPtr<asIScriptContext> m_Context;

	CASObjPtr m_ConfigurationObject;

private:
	CASConfigModuleBuilder( const CASConfigModuleBuilder& ) = delete;
	CASConfigModuleBuilder& operator=( const CASConfigModuleBuilder& ) = delete;
};

#endif //ANGELSCRIPT_CASCONFIGMODULEBUILDER_H
