#ifndef __GOALSETVIEW__
#define __GOALSETVIEW__

#include <QGroupBox>
#include <QVBoxLayout>
#include <QMenu>
#include <QList>
#include <QScrollArea>
#include <widgets/GoalView.hxx>

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

	QVBoxLayout*		mViewLayout;
	QMenu*			mMenu;
	QAction*		mCreateNewGoal;
	QAction*		mClearGoals;
	bool			mFrozen;
	QScrollArea*		mGoalsArea;
	QVBoxLayout*		mGoalsAreaLayout;
	QList< GoalView* >	mGoalViews;
};

}

#endif // GoalSetView.hxx
