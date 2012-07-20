#ifndef __OBSERVER__
#define __OBSERVER__

#include <QObject>
#include <planning/STRIPS_Problem.hxx>
#include <vector>
#include <list>
#include <utility>
#include <planning/Goal.hxx>
#include <widgets/GoalView.hxx>

using aig_tk::STRIPS_Problem;
using aig_tk::Action_Ptr_Vec;

namespace Planning
{

class Observer : public QObject
{
	Q_OBJECT
public:

	Observer( STRIPS_Problem& domain );
	virtual ~Observer();

	bool	started() const { return mStarted; }
	float	obsLevel() const { return mObsLevel; }

signals:
	void	requestInitialStateUpdate();
	void	goalViewCreated( UI::GoalView* v );
	void	sendHypGoalsInfo( QList< QString >&, QList< QString >& );
	void	actionObserved( QString a );

public slots:
	void	start();
	void	stop();
	void	setObsLevel( float level );
	void	actionExecuted( unsigned index );
	void	updateInitialState( aig_tk::Fluent_Vec& eval );
	void	addNewGoal( QString label, QList< QString >& fluents );
	void	removeGoal( QString label );
	void	onHypGoalsInfoRequested();

protected:

	STRIPS_Problem&		mDomain;
	Action_Ptr_Vec		mObservations;
	bool			mStarted;
	float			mObsLevel;
	aig_tk::Fluent_Vec	mInitialState;
	std::list< Goal* >	mHypGoals;
	
};

}

#endif
