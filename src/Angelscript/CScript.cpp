#include <algorithm>
#include <fstream>

#include "CScript.h"

CScript::CScript( std::string&& szSectionName, std::shared_ptr<const CConfiguration> config )
	: m_szSectionName( std::move( szSectionName ) )
	, m_Configuration( config )
{
}

CScript::CScript( std::string&& szSectionName, std::string&& szContents, std::shared_ptr<const CConfiguration> config )
	: m_szSectionName( std::move( szSectionName ) )
	, m_szContents( std::move( szContents ) )
	, m_Configuration( config )
{
}

CScript CScript::FromFile( const std::string& szFileName, const std::string& szSectionName, std::shared_ptr<const CConfiguration> config )
{
	CScript script( std::string( szSectionName.empty() ? szFileName : szSectionName ), config );

	script.LoadFromFile( szFileName );

	return script;
}

void CScript::SetSectionName( std::string&& szSectionName )
{
	m_szSectionName = std::move( szSectionName );
}

void CScript::SetContents( std::string&& szContents )
{
	m_szContents = std::move( szContents );
}

std::string CScript::LoadContentsFromFile( const std::string& szFileName, bool& bSuccess )
{
	std::ifstream inStream( szFileName, std::ifstream::in | std::ifstream::binary );

	if( !inStream.is_open() )
	{
		bSuccess = false;
		return {};
	}

	//Read in the whole file
	inStream.seekg( 0, std::ifstream::end );

	std::string szContents;

	szContents.resize( static_cast<size_t>( inStream.tellg() ) );

	inStream.seekg( 0, std::ifstream::beg );

	inStream.read( &szContents[ 0 ], szContents.size() );

	inStream.close();

	//Normalize newlines
	szContents.erase( std::remove( szContents.begin(), szContents.end(), '\r' ), szContents.end() );

	bSuccess = true;

	return szContents;
}

std::string CScript::LoadContentsFromFile( const std::string& szFileName )
{
	bool bSuccess;

	return LoadContentsFromFile( szFileName, bSuccess );
}

bool CScript::LoadFromFile( const std::string& szFileName )
{
	bool bSuccess;

	m_szContents = LoadContentsFromFile( szFileName, bSuccess );

	return bSuccess;
}

bool CScript::SaveToFile( const std::string& szFileName )
{
	std::ofstream outStream( szFileName, std::ios_base::out | std::ios_base::trunc );

	if( !outStream.is_open() )
		return false;

	outStream << m_szContents;

	outStream.close();

	return true;
}
