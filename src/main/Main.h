#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include <QObject>

#include "ide/CASIDEApp.h"

class CLauncher final : public QObject
{
	Q_OBJECT

public:
	std::shared_ptr<CASIDEApp> m_App;

	void CLauncher::OnQuit()
	{
		m_App->Shutdown();
	}
};

#endif //MAIN_H
