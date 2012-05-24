#ifndef __PLANNING_TASK__
#define __PLANNING_TASK__

#include <QObject>
#include <planning/STRIPS_Problem.hxx>
#include <planning/Action.hxx>

using aig_tk::STRIPS_Problem;
using aig_tk::Action;
using aig_tk::Action_Ptr_Vec;


namespace Planning
{

class	PlanningTask : public QObject
{
	Q_OBJECT
public:

	PlanningTask( STRIPS_Problem& prob, Action_Ptr_Vec& obs, bool doReachabilityTest = false );
	virtual ~PlanningTask();

	aig_tk::Cost_Type	result() const;
	
	bool			doReachabilityTest();
	static void		solve( PlanningTask* task );

protected:

	STRIPS_Problem&		mProblem;
	Action_Ptr_Vec&		mObsSequence;
	aig_tk::Cost_Type	mCost;
	bool			mDoReachabilityTest;
};

}

#endif // PlanningTask.hxx
