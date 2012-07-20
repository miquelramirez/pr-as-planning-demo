#ifndef __GOALSETVIEW__
#define __GOALSETVIEW__

#include <QGroupBox>
#include <QVBoxLayout>
#include <QMenu>
#include <QList>
#include <QFrame>
#include <QScrollArea>
#include <widgets/GoalView.hxx>
#include <widgets/ObservationsView.hxx>

namespace UI
{

class GoalSetView : public QGroupBox
{
	Q_OBJECT
public:
	GoalSetView( QWidget* parent = NULL );
	GoalSetView( const QString& title, QWidget* parent = NULL );
	virtual ~GoalSetView();

signals:
	
	void	requestHypGoalsInfo();
	void	createNewGoal( QString name, QList< QString >& fluents );

public slots:

	void	createNewGoalRequested();
	void	acceptHypGoalsInfo( QList< QString >& fluents, QList< QString >& goals );

	void	addNewGoalView( UI::GoalView* v );
	void	removeGoalView( QString name );
	void	clearGoals();
	void	freeze();
	void	unFreeze();

protected:

	void		setup();
	virtual void	mousePressEvent ( QMouseEvent * event );
protected:

	QMenu*			mMenu;
	QAction*		mCreateNewGoal;
	QAction*		mClearGoals;
	bool			mFrozen;
	QVBoxLayout*		mViewLayout;
	QFrame*			mGoalsList;
	QVBoxLayout*		mGoalListLayout;
	QScrollArea*		mGoalsScrollingManager;
	QList< GoalView* >	mGoalViews;
};

}

#endif // GoalSetView.hxx
