#ifndef ICOMPILERLISTENER_H
#define ICOMPILERLISTENER_H

struct asSMessageInfo;

class IASCompilerListener
{
public:

	virtual ~IASCompilerListener() = 0;

	virtual void CompilerMessage( const asSMessageInfo* pMsg ) = 0;
};

inline IASCompilerListener::~IASCompilerListener()
{
}

#endif // ICOMPILERLISTENER_H

