#ifndef UTIL_IUI_H
#define UTIL_IUI_H

#include <memory>
#include <string>

enum class UIMessageType
{
	Info,
	Warning,
	Error
};

class IUI
{
public:
	virtual ~IUI() = 0;

	virtual void SendMessage( const char* pszString, UIMessageType type = UIMessageType::Info ) = 0;
	virtual void SendMessage( const std::string& szString, UIMessageType type = UIMessageType::Info ) = 0;
};

inline IUI::~IUI()
{
}

#endif //UTIL_IUI_H
