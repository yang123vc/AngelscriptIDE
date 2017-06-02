#ifndef ANGELSCRIPT_CCONFIGURATIONEXCEPTION_H
#define ANGELSCRIPT_CCONFIGURATIONEXCEPTION_H

#include <stdexcept>

class CConfigurationException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

#endif //ANGELSCRIPT_CCONFIGURATIONEXCEPTION_H
