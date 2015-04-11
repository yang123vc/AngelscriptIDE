#ifndef CBASEOUTPUTWIDGET_H
#define CBASEOUTPUTWIDGET_H

#include <memory>
#include <QWidget>

class CUI;

class CBaseOutputWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CBaseOutputWidget( const QString& szName, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	virtual ~CBaseOutputWidget();

	const QString& GetName() const { return m_szName; }

protected:
	std::shared_ptr<CUI> m_UI;

private:
	QString m_szName;
};

#endif // CBASEOUTPUTWIDGET_H
