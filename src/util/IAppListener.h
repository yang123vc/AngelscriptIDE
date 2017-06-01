#ifndef IAPPLISTENER_H
#define IAPPLISTENER_H

class IAppListener
{
public:
	virtual ~IAppListener() = 0;

	virtual void AppStartedUp() = 0;

	virtual void AppShutdown() = 0;
};

inline IAppListener::~IAppListener()
{
}

#endif // IAPPLISTENER_H

