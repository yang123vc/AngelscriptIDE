#ifndef CUI_H
#define CUI_H

#include <memory>
#include <string>

#include <QString>

#include "util/CListenerManager.h"
#include "util/IUI.h"

class CASIDEApp;

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
class CUI final : public IUI
{
public:
	CUI();
	~CUI();

	void AddUIEventListener( IUIEventListener* pListener );
	void RemoveUIEventListener( IUIEventListener* pListener );

	/*
	 * Sends a message
	 * Listeners can choose to display this string
	*/
	void SendMessage( const char* pszString, UIMessageType type = UIMessageType::Info ) override;
	void SendMessage( const std::string& szString, UIMessageType type = UIMessageType::Info ) override;
	void SendMessage( const QString& szString, UIMessageType type = UIMessageType::Info );

private:
	std::shared_ptr<CASIDEApp> m_App;
	CListenerManager<IUIEventListener> m_UIListeners;

private:
	CUI( const CUI& );
	CUI& operator=( const CUI& );
};

#endif // CUI_H