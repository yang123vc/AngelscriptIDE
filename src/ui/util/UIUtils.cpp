#include <QAction>

#include "UIUtils.h"

bool operator==( QAction* pAction, const std::string& szFilename )
{
	return pAction->text().toStdString() == szFilename;
}

namespace ui
{
const char* ANY_EXTENSION = "*.*";
}
