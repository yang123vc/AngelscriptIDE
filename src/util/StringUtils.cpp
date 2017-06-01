#include "StringUtils.h"

std::vector<std::string>& Split( const std::string& szString, char cDelim, std::vector<std::string>& vecResults )
{
	std::stringstream stream( szString );

	std::string szItem;

	while( std::getline( stream, szItem, cDelim ) )
		vecResults.push_back( szItem );

	return vecResults;
}

std::vector<std::string> Split( const std::string& szString, char cDelim )
{
	std::vector<std::string> vecResults;

	return Split( szString, cDelim, vecResults );
}

std::string Implode( const std::vector<std::string>& vecStrings, char cDelim )
{
	std::stringstream stream;

	if( !vecStrings.empty() )
	{
		stream << vecStrings[ 0 ];

		const auto uiSize = vecStrings.size();

		if( uiSize > 1 )
		{
			for( size_t uiIndex = 1; uiIndex < uiSize; ++uiIndex )
				stream << cDelim << vecStrings[ uiIndex ];
		}
	}

	return stream.str();
}
