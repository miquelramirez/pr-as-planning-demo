#include <planning/PlanningThread.hxx>
#include <limits>
#include <planning/Types.hxx>
#include <search/Node.hxx>
#include <planning/Single_Queue_FF_BFS.hxx>
#include <util/time.hxx>
#include <util/ext_math.hxx>
#include <planning/inference/Reachability_Test.hxx>

namespace Planning
{

PlanningThread::PlanningThread( STRIPS_Problem& mProblem )
	: mProblem( mProblem ), mCost( std::numeric_limits<aig_tk::Cost_Type>::infinity() )
{

}

PlanningThread::~PlanningThread()
{
}

void	PlanningThread::run()
{
	solve();
	emit finished();	
}

void	PlanningThread::solve()
{
	float t0, tf;

	t0 = time_used();
	aig_tk::Node* n0 = aig_tk::Node::root( mProblem );
	std::vector<aig_tk::Node*> plan;
	aig_tk::Single_Queue_FF_BFS engine;
	engine.init( mProblem, n0 );
	engine.start();
	float maxTime = 0.1f;
	float runningTime = 0.0f;	

	bool solved;
	do
	{
		plan.clear();
		engine.set_budget( maxTime - runningTime );
		solved = engine.findSolution(plan);
		tf = time_used();
		std::cout << "Plan computed:"; report_interval( t0, tf, std::cout ); std::cout << std::endl;
	
		if( solved )
			mCost = plan.back()->gn();
		runningTime += (tf - t0);
		t0 = time_used();
		std::cout << "Running Time: " << runningTime << std::endl;
	} while ( solved && ( runningTime <= maxTime ) );
	std::cout << "Total Running Time: " << runningTime << std::endl;
}

}
