#ifndef __PLANNING_THREAD__
#define __PLANNING_THREAD__

#include <QThread>
#include <planning/STRIPS_Problem.hxx>

using aig_tk::STRIPS_Problem;


namespace Planning
{

class	PlanningThread : public QThread
{
	Q_OBJECT
public:

	PlanningThread( STRIPS_Problem& prob );
	virtual ~PlanningThread();

	aig_tk::Cost_Type	result() const { return mCost; }

protected:

	virtual void 	run();
	void		solve();


protected:

	STRIPS_Problem&		mProblem;
	aig_tk::Cost_Type	mCost;
};

}

#endif // PlanningThread.hxx
