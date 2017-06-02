#ifndef LAUNCHER_CLAUNCHEREXCEPTION_H
#define LAUNCHER_CLAUNCHEREXCEPTION_H

#include <stdexcept>

class CLauncherException : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

#endif //LAUNCHER_CLAUNCHEREXCEPTION_H
