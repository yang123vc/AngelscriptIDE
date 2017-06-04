#ifndef UTIL_IAPPLISTENER_H
#define UTIL_IAPPLISTENER_H

class IAppListener
{
public:
	virtual ~IAppListener() = 0;

	virtual void AppStartedUp() = 0;

	virtual void OnBeforeRun() {}

	virtual void AppShutdown() = 0;
};

inline IAppListener::~IAppListener()
{
}

#endif //UTIL_IAPPLISTENER_H
