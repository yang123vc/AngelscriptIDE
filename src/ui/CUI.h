#ifndef IDE_UI_CUI_H
#define IDE_UI_CUI_H

#include <memory>
#include <string>

#include <QString>

#include "util/CListenerManager.h"

class CASIDEApp;

enum class UIMessageType
{
	INFO,
	WARNING,
	ERROR
};

class IUIEventListener
{
public:

	virtual ~IUIEventListener() = 0;

	virtual void ReceiveUIMessage( const char* pszString, UIMessageType type );
};

inline IUIEventListener::~IUIEventListener()
{
}

inline void IUIEventListener::ReceiveUIMessage( const char*, UIMessageType )
{
}

/*
 * This class represents the UI itself
*/
class CUI final
{
public:
	CUI();
	~CUI();

	void AddUIEventListener( IUIEventListener* pListener );
	void RemoveUIEventListener( IUIEventListener* pListener );

	/**
	*	Sends a message
	*	Listeners can choose to display this string
	*/
	void SendMessage( const char* pszString, UIMessageType type = UIMessageType::INFO );
	void SendMessage( const std::string& szString, UIMessageType type = UIMessageType::INFO );
	void SendMessage( const QString& szString, UIMessageType type = UIMessageType::INFO );

private:
	std::shared_ptr<CASIDEApp> m_App;
	CListenerManager<IUIEventListener> m_UIListeners;

private:
	CUI( const CUI& ) = delete;
	CUI& operator=( const CUI& ) = delete;
};

#endif //IDE_UI_CUI_H
