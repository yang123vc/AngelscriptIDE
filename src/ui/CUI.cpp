#include "CUI.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

void CUI::SendMessage( const char* pszString, UIMessageType type )
{
	if( !pszString )
		return;

	UIMessage( pszString, type );
}

void CUI::SendMessage( const std::string& szString, UIMessageType type )
{
	SendMessage( szString.c_str(), type );
}

void CUI::SendMessage( const QString& szString, UIMessageType type )
{
	SendMessage( szString.toStdString(), type );
}
