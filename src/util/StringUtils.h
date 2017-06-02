#ifndef UTIL_STRINGUTILS_H
#define UTIL_STRINGUTILS_H

#include <vector>
#include <string>
#include <sstream>

std::vector<std::string>& Split( const std::string& szString, char cDelim, std::vector<std::string>& vecResults );

std::vector<std::string> Split( const std::string& szString, char cDelim );

template<typename T>
T& Split( const std::string& szString, char cDelim, T& results )
{
	std::stringstream stream( szString );

	std::string szItem;

	while( std::getline( stream, szItem, cDelim ) )
		results.push_back( szItem );

	return results;
}

template<typename T>
T Split( const std::string& szString, char cDelim )
{
	T results;

	return Split( szString, cDelim, results );
}

std::string Implode( const std::vector<std::string>& vecStrings, char cDelim );

template<typename T>
std::string Implode( const T& strings, char cDelim )
{
	std::stringstream stream;

	if( !strings.empty() )
	{
		auto it = strings.begin();

		stream << *it;

		++it;

		auto end = strings.end();

		for( ; it != end; ++it )
			stream << cDelim << *it;
	}

	return stream.str();
}

#endif //UTIL_STRINGUTILS_H
