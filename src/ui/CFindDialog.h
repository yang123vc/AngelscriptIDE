#ifndef IDE_UI_CFINDDIALOG_H
#define IDE_UI_CFINDDIALOG_H

#include <memory>

#include <QDialog>

namespace Ui
{
class CFindDialog;
}

class CFindDialog : public QDialog
{
	Q_OBJECT

private:

	static const char* SEARCH_DIRECTION_PROPERTY;

public:

	enum class SearchDirection
	{
		FORWARD,
		BACKWARD
	};

public:
	explicit CFindDialog( QWidget* pParent = nullptr );
	~CFindDialog();

	bool MatchWholeWord() const;
	bool MatchCase() const;

	SearchDirection GetSearchDirection() const;

signals:

	void Find( const QString& szString );

protected slots:

	void FindNext();

	void Close();

private:
	std::unique_ptr<Ui::CFindDialog> m_WidgetUI;
};

#endif //IDE_UI_CFINDDIALOG_H
