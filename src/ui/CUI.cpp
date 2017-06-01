#include "CUI.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

void CUI::AddUIEventListener( IUIEventListener* pListener )
{
	m_UIListeners.AddListener( pListener );
}

void CUI::RemoveUIEventListener( IUIEventListener* pListener )
{
	m_UIListeners.RemoveListener( pListener );
}

void CUI::SendMessage( const char* pszString, UIMessageType type )
{
	if( !pszString )
		return;

	m_UIListeners.NotifyListeners( &IUIEventListener::ReceiveUIMessage, pszString, type );
}

void CUI::SendMessage( const std::string& szString, UIMessageType type )
{
	SendMessage( szString.c_str(), type );
}

void CUI::SendMessage( const QString& szString, UIMessageType type )
{
	SendMessage( szString.toStdString(), type );
}
