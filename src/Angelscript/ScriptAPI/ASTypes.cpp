#include <stdexcept>

#include <angelscript.h>

#include "ASTypes.h"

void RegisterASTypes( asIScriptEngine& scriptEngine )
{
	const char* pszPWordType = nullptr;

	switch( sizeof( asPWORD ) )
	{
	case 1: pszPWordType = "uint8"; break;
	case 2: pszPWordType = "uint16"; break;
	case 4: pszPWordType = "uint32"; break;
	case 8: pszPWordType = "uint64"; break;

		//Better than stack corruption
	default: throw std::runtime_error( "Couldn't determine size of asPWORD" );
	}

	scriptEngine.RegisterTypedef( "asPWORD", pszPWordType );
}
