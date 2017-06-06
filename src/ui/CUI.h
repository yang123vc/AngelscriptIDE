#ifndef IDE_UI_CUI_H
#define IDE_UI_CUI_H

#include <memory>
#include <string>

#include <QObject>
#include <QString>

class CASIDEApp;

//TODO: remove Windows dependency - Solokiller
#undef ERROR

enum class UIMessageType
{
	INFO,
	WARNING,
	ERROR
};

/*
 * This class represents the UI itself
*/
class CUI final : public QObject
{
	Q_OBJECT

public:
	CUI();
	~CUI();

	/**
	*	Sends a message
	*	Listeners can choose to display this string
	*/
	void SendMessage( const char* pszString, UIMessageType type = UIMessageType::INFO );
	void SendMessage( const std::string& szString, UIMessageType type = UIMessageType::INFO );
	void SendMessage( const QString& szString, UIMessageType type = UIMessageType::INFO );

signals:
	void UIMessage( const char* pszString, UIMessageType type );

private:
	std::shared_ptr<CASIDEApp> m_App;

private:
	CUI( const CUI& ) = delete;
	CUI& operator=( const CUI& ) = delete;
};

#endif //IDE_UI_CUI_H
