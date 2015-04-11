#ifndef LAUNCHEREXCEPTION_H
#define LAUNCHEREXCEPTION_H

#include <stdexcept>

class LauncherException : public std::logic_error
{
public:
	explicit LauncherException( const std::string& szArg )
		: std::logic_error( szArg )
	{

	}
};

#endif // LAUNCHEREXCEPTION_H
