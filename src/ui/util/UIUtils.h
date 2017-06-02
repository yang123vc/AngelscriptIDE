#ifndef UI_UTIL_UIUTILS_H
#define UI_UTIL_UIUTILS_H

#include <stdexcept>
#include <string>

#include <QString>

class QAction;

bool operator==( QAction* pAction, const std::string& szFilename );

namespace ui
{
/*
 * Constants used by the UI
*/
extern const char* ANY_EXTENSION;

/*
 * Converts any supported string type to a QString
*/
inline QString ConvertToQString( const char*& pszString )
{
	return pszString;
}

inline QString ConvertToQString( const std::string& szString )
{
	return szString.c_str();
}

inline QString ConvertToQString( const QString& szString )
{
	return szString;
}

/*
 * Formats the given extensions as a string that can be used in file dialogs
 * If no extensions are in the given container, the extensions will be formatted as "*.*"
*/
template<typename T>
QString FormatExtensions( const T& extensions )
{
	QString szExtensions = ANY_EXTENSION;

	if( !extensions.empty() )
	{
		auto it = extensions.begin();

		szExtensions = QString( "*%1" ).arg( ConvertToQString( *it ) );

		++it;

		auto end = extensions.end();

		for( ; it != end; ++it )
			szExtensions += QString( " *%1" ).arg( ConvertToQString( *it ) );
	}

	return szExtensions;
}
}

#endif //UI__UTIL_UIUTILS_H
