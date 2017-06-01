#include <fstream>

#include "CScriptFile.h"

CScriptFile::CScriptFile( const std::string& szFilename )
	: m_szFilename( szFilename )
{

}

CScriptFile::~CScriptFile()
{

}

void CScriptFile::SetFilename( const std::string& szFilename )
{
	m_szFilename = szFilename;
}

std::unique_ptr<std::string> CScriptFile::GetScriptContents() const
{
	std::ifstream inStream( m_szFilename );

	if( !inStream.is_open() )
		return std::unique_ptr<std::string>();

	inStream.seekg( 0, inStream.end );

	auto uiSize = static_cast<size_t>( inStream.tellg() );

	inStream.seekg( 0, inStream.beg );

	std::unique_ptr<std::string> buffer( new std::string() );

	buffer->reserve( uiSize );

	std::string szLine;

	bool fFirstLine = true;

	while( inStream.good() )
	{
		std::getline( inStream, szLine );

		if( fFirstLine )
			fFirstLine = false;
		else
			*buffer += '\n';

		*buffer += szLine;
	}

	inStream.close();

	return buffer;
}

bool CScriptFile::SetScriptContents( const std::string& szContents )
{
	std::ofstream outStream( m_szFilename, std::ios_base::out | std::ios_base::trunc );

	if( !outStream.is_open() )
		return false;

	outStream << szContents;

	outStream.close();

	return true;
}
