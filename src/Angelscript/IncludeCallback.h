#ifndef ANGELSCRIPT_INCLUDECALLBACK_H
#define ANGELSCRIPT_INCLUDECALLBACK_H

#include <memory>

class CConfiguration;
class CScriptBuilder;
class QString;

/**
*	Finds an included file, given a configuration that contains a fallback extension and a list of include paths
*/
int FindIncludedFile( CScriptBuilder& builder, const std::shared_ptr<const CConfiguration>& config,
					  const QString& szInclude, const QString& szFrom );

#endif //ANGELSCRIPT_INCLUDECALLBACK_H
