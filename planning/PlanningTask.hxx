#ifndef __PLANNING_TASK__
#define __PLANNING_TASK__

#include <QObject>
#include <strips_prob.hxx>
#include <action.hxx>

using aptk::STRIPS_Problem;
using aptk::Action;
using aptk::Action_Ptr_Vec;


namespace Planning
{

class	PlanningTask : public QObject
{
	Q_OBJECT
public:

	PlanningTask( STRIPS_Problem& prob, Action_Ptr_Vec& obs, bool doReachabilityTest = false );
	virtual ~PlanningTask();

	aptk::Cost_Type	result() const;
	
	bool			doReachabilityTest();
	static void		solve( PlanningTask* task );

protected:

	STRIPS_Problem&		mProblem;
	Action_Ptr_Vec&		mObsSequence;
	aptk::Cost_Type	mCost;
	bool			mDoReachabilityTest;
};

}

#endif // PlanningTask.hxx
