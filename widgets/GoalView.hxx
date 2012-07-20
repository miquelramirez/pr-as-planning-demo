#ifndef __GOAL_VIEW__
#define __GOAL_VIEW__

#include <QFrame>
#include <QProgressBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>

namespace UI
{

class GoalView : public QFrame
{
	Q_OBJECT
public:
	GoalView( const QString& title, QWidget* parent = NULL );

	virtual ~GoalView();

	const QString&	goalName() { return mGoalName; }
	void		makeTooltip( const QStringList& list );
signals:
	
	void 	removeGoal( QString label );
	
public slots:

	void 	setProbability( float v );
	void	removalRequested();

protected:

	void 	setupWidgets();

protected:
	QString		mGoalName;
	QVBoxLayout*	mLayout;
	QLabel*		mGoalNameLabel;
	QLabel*		mProbDisplayLabel;
	QProgressBar*	mProbDisplay;
	QMenu*		mPopupMenu;
	QAction*	mRemoveAction;
	QWidget*	mProbView;
};

}

#endif // GoalView.hxx
