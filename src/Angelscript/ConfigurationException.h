#ifndef ANGELSCRIPT_CONFIGURATIONEXCEPTION_H
#define ANGELSCRIPT_CONFIGURATIONEXCEPTION_H

#include <stdexcept>

class ConfigurationException : public std::runtime_error
{
public:
	explicit ConfigurationException( const std::string& szArg )
		: std::runtime_error( szArg )
	{
	}
};

#endif //ANGELSCRIPT_CONFIGURATIONEXCEPTION_H
