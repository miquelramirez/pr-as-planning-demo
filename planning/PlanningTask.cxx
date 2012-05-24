#include <planning/PlanningTask.hxx>
#include <limits>
#include <planning/Types.hxx>
#include <search/Node.hxx>
#include <planning/Single_Queue_FF_BFS.hxx>
#include <util/time.hxx>
#include <util/ext_math.hxx>
#include <planning/inference/Reachability_Test.hxx>

using aig_tk::Fluent_Vec;

namespace Planning
{

PlanningTask::PlanningTask( STRIPS_Problem& problem, Action_Ptr_Vec& obs, bool doReachabilityTest )
	: mProblem( problem ), mObsSequence(obs), mCost( std::numeric_limits<aig_tk::Cost_Type>::infinity() ),
	mDoReachabilityTest( doReachabilityTest )
{

}

PlanningTask::~PlanningTask()
{
}

aig_tk::Cost_Type	PlanningTask::result() const
{
	return mCost;
}

bool	PlanningTask::doReachabilityTest( )
{

	aig_tk::Reachability_Test	rtest( mProblem );
	
	if (rtest.is_reachable( mProblem.init(), mProblem.goal(), mObsSequence.back()->index() ) )
		return true;
	for ( unsigned i = 0; i < mObsSequence.size()-1; i++ )
	{
	
		Fluent_Vec& prec_i = mProblem.actions()[mObsSequence[i+1]->index()]->prec_vec();

		if( rtest.is_reachable( mProblem.init(), prec_i, mObsSequence[i]->index() ) )
			return true;
	}
	return false;

}

void	PlanningTask::solve( PlanningTask* task )
{
	if ( task->mDoReachabilityTest )
	{
		if ( !task->doReachabilityTest( ) )
		{
			task->mCost = std::numeric_limits<aig_tk::Cost_Type>::infinity();
			return;
		}
	}

	float t0, tf;

	t0 = time_used();
	aig_tk::Node* n0 = aig_tk::Node::root( task->mProblem );
	std::vector<aig_tk::Node*> plan;
	aig_tk::Single_Queue_FF_BFS engine;
	engine.init( task->mProblem, n0 );
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
			task->mCost = plan.back()->gn();
		runningTime += (tf - t0);
		t0 = time_used();
		std::cout << "Running Time: " << runningTime << std::endl;
	} while ( solved && ( runningTime <= maxTime ) );
	std::cout << "Total Running Time: " << runningTime << std::endl;
}

}
