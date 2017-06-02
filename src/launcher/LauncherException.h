#ifndef LAUNCHER_LAUNCHEREXCEPTION_H
#define LAUNCHER_LAUNCHEREXCEPTION_H

#include <stdexcept>

class LauncherException : public std::logic_error
{
public:
	explicit LauncherException( const std::string& szArg )
		: std::logic_error( szArg )
	{

	}
};

#endif //LAUNCHER_LAUNCHEREXCEPTION_H
