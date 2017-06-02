#ifndef ANGELSCRIPT_ICOMPILERLISTENER_H
#define ANGELSCRIPT_ICOMPILERLISTENER_H

struct asSMessageInfo;

/**
*	Enables listening to compiler messages
*/
class IASCompilerListener
{
public:

	virtual ~IASCompilerListener() = 0;

	virtual void CompilerMessage( const asSMessageInfo* pMsg ) = 0;
};

inline IASCompilerListener::~IASCompilerListener()
{
}

#endif //ANGELSCRIPT_ICOMPILERLISTENER_H
