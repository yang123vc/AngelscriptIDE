#ifndef IDE_UI_OPTIONS_CBASEOPTIONSWIDGET_H
#define IDE_UI_OPTIONS_CBASEOPTIONSWIDGET_H

#include <memory>

#include <QWidget>

class CASIDEApp;
class COptionsDialog;
class CUI;

/**
*	Base class for options pages.
*/
class CBaseOptionsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CBaseOptionsWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent );
	~CBaseOptionsWidget();

	virtual bool CanSave( QString& szReason ) = 0;

	virtual void ApplyChanges() = 0;

	virtual void PostApply() {}

	bool HaveChangesBeenMade() const { return m_bChangesMade; }

	void SetChangesMade( bool bState );

signals:

	void ChangesMade( bool bState );

private:
	void ConnectToParent();

protected:
	std::shared_ptr<CASIDEApp> m_App;
	std::shared_ptr<CUI> m_UI;
	COptionsDialog* m_pParent;

private:
	bool m_bChangesMade = false;
};

#endif //IDE_UI_OPTIONS_CBASEOPTIONSWIDGET_H
