#include "CConfiguration.h"

CConfiguration::CConfiguration( const std::string& szName )
{
	SetName( szName );
}

void CConfiguration::SetName( const std::string& szName )
{
	m_szName = szName;
}

void CConfiguration::SetConfigFilename( const std::string& szConfigFilename )
{
	m_szConfigFilename = szConfigFilename;
}

void CConfiguration::SetConfigScriptFilename( std::string&& szConfigScriptFilename )
{
	m_szConfigScriptFilename = std::move( szConfigScriptFilename );
}

void CConfiguration::SetIncludeFilename( const std::string& szIncludeFilename )
{
	m_szIncludeFilename = szIncludeFilename;
}

void CConfiguration::SetFallbackExtension( const std::string& szFallbackExtension )
{
	m_szFallbackExtension = szFallbackExtension;
}
