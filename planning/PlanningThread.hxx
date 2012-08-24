#ifndef __PLANNING_THREAD__
#define __PLANNING_THREAD__

#include <QThread>
#include <strips_prob.hxx>

using aptk::STRIPS_Problem;


namespace Planning
{

class	PlanningThread : public QThread
{
	Q_OBJECT
public:

	PlanningThread( STRIPS_Problem& prob );
	virtual ~PlanningThread();

	aptk::Cost_Type	result() const { return mCost; }

protected:

	virtual void 	run();
	void		solve();


protected:

	STRIPS_Problem&		mProblem;
	aptk::Cost_Type	mCost;
};

}

#endif // PlanningThread.hxx
