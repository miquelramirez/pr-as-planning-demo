#ifndef __OBSERVATIONS_VIEW__
#define __OBSERVATIONS_VIEW__

#include <QListView>
#include <QStringListModel>
#include <QGroupBox>
#include <QString>

namespace UI
{

class ObservationsView : public QGroupBox
{
	Q_OBJECT

public:

	ObservationsView( QWidget* parent = NULL );
	ObservationsView( const QString& title, QWidget* parent = NULL );
	virtual ~ObservationsView();

public slots:

	void	logAction( QString signature );
	void	clearActionLog();

protected:
	
	void	setup();

protected:

	QListView*		mActionLogView;
	QStringListModel	mActionLog;
	

};


}

#endif // ObservationsView.hxx 
