#include <fstream>

#include "CProperties.h"

CProperties::CProperties()
{

}

CProperties::~CProperties()
{

}

bool CProperties::Load( const std::string& szFilename )
{
	m_Keyvalues.clear();

	std::ifstream inStream( szFilename );

	if( !inStream.is_open() )
		return false;

	std::string szLine;

	while( inStream )
	{
		if( std::getline( inStream, szLine ) )
		{
			auto uiSeparator = szLine.find( '=' );

			if( uiSeparator != std::string::npos )
			{
				std::string szKey = szLine.substr( 0, uiSeparator );

				std::string szValue = szLine.substr( uiSeparator + 1, szLine.length() - uiSeparator - 1 );

				m_Keyvalues.insert( { szKey, szValue } );
			}
		}
	}

	return true;
}

bool CProperties::Save( const std::string& szFilename )
{
	std::ofstream outStream( szFilename );

	if( !outStream.is_open() )
		return false;

	Keyvalues_t::const_iterator end = m_Keyvalues.end();

	for( Keyvalues_t::const_iterator it = m_Keyvalues.begin(); it != end; ++it )
	{
		outStream << it->first << '=' << it->second << std::endl;
	}

	return true;
}
