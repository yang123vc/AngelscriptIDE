#include "CScript.h"

CScript::CScript( const std::string& szSectionName, const std::string& szContents, std::shared_ptr<const CConfiguration> config )
	: m_szSectionName( szSectionName )
	, m_szContents( szContents )
	, m_Configuration( config )
{

}

CScript::~CScript()
{

}

CScript::CScript( const CScript& other )
{
	Copy( other );
}

CScript& CScript::operator=( const CScript& other )
{
	Copy( other );

	return *this;
}

void CScript::Copy( const CScript& other )
{
	if( this != &other )
	{
		m_szSectionName = other.m_szSectionName;
		m_szContents = other.m_szContents;
		m_Configuration = other.m_Configuration;
	}
}
