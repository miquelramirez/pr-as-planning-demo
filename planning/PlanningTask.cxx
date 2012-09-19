/*
Plan Recognition as Planning Demo: Home domain
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <planning/PlanningTask.hxx>
#include <limits>
#include <types.hxx>
#include <aptk/resources_control.hxx>
#include <aptk/ext_math.hxx>
#include <reachability.hxx>
#include <strips_state.hxx>
#include <fwd_search_prob.hxx>
#include <h_1.hxx>
#include <rp_heuristic.hxx>
#include <simple_landmarks.hxx>
#include <reachability.hxx>
#include <aptk/open_list.hxx>
#include <aptk/at_rwbfs_dq_mh.hxx>

using aptk::Fluent_Vec;

namespace Planning
{

using	aptk::State;
using	aptk::agnostic::Fwd_Search_Problem;

using 	aptk::agnostic::H1_Heuristic;
using	aptk::agnostic::H_Add_Evaluation_Function;
using	aptk::agnostic::Relaxed_Plan_Heuristic;
using	aptk::agnostic::Simple_Landmarks_Heuristic;

using	aptk::agnostic::Reachability_Test;

using 	aptk::search::Open_List;
using	aptk::search::Node_Comparer_DH;
using 	aptk::search::bfs_dq_mh::Node;
using	aptk::search::bfs_dq_mh::AT_RWBFS_DQ_MH;

typedef		Node< State >									Search_Node;
typedef		Node_Comparer_DH< Search_Node >							Tie_Breaking_Algorithm;
typedef		Open_List< Tie_Breaking_Algorithm, Search_Node >				BFS_Open_List;
typedef		H1_Heuristic<Fwd_Search_Problem, H_Add_Evaluation_Function>			H_Add_Fwd;
typedef		Relaxed_Plan_Heuristic< Fwd_Search_Problem, H_Add_Fwd >				H_Add_Rp_Fwd;
typedef		Simple_Landmarks_Heuristic< Fwd_Search_Problem >				H_LM;
typedef		AT_RWBFS_DQ_MH< Fwd_Search_Problem, H_Add_Rp_Fwd, H_LM, BFS_Open_List >		Anytime_RWBFS_H_Add_Rp_Fwd;

PlanningTask::PlanningTask( STRIPS_Problem& problem, Action_Ptr_Vec& obs, std::string logFileName, bool doReachabilityTest )
	: mProblem( problem ), mObsSequence(obs), mCost( std::numeric_limits<aptk::Cost_Type>::infinity() ),
	mDoReachabilityTest( doReachabilityTest )
{
	mLogFile.open( logFileName.c_str() );
}

PlanningTask::~PlanningTask()
{
	mLogFile.close();
}

aptk::Cost_Type	PlanningTask::result() const
{
	return mCost;
}

bool	PlanningTask::doReachabilityTest( )
{

	Reachability_Test	rtest( mProblem );
	
	if (rtest.is_reachable( mProblem.init(), mProblem.goal(), mObsSequence.back()->index() ) )
		return true;
	for ( int i = mObsSequence.size()-1; i > 0; i-- )
	{
	
		const Fluent_Vec& prec_i = mProblem.actions()[mObsSequence[i]->index()]->prec_vec();

		if( rtest.is_reachable( mProblem.init(), prec_i, mObsSequence[i-1]->index() ) )
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
			task->mCost = std::numeric_limits<aptk::Cost_Type>::infinity();
			task->mLogFile << "Reachability Test Failed: Task is not solvable" << std::endl;
			return;
		}
	}
	task->mLogFile << "Search started" << std::endl;
	Fwd_Search_Problem		search_prob( &task->mProblem );
	Anytime_RWBFS_H_Add_Rp_Fwd	engine( search_prob );
	
	engine.set_schedule( 10, 5, 1 );
	engine.start();
	float t0, tf;
	std::vector< aptk::Action_Idx > plan;
	float				cost;

	t0 = aptk::time_used();
	float maxTime = 3.0f;
	float runningTime = 0.0f;	
	unsigned expanded_0 = engine.expanded();
	unsigned generated_0 = engine.generated();

	bool solved;
	do
	{
		plan.clear();
		engine.set_budget( maxTime );// - runningTime );
		
		solved = engine.find_solution(cost, plan);
		tf = aptk::time_used();
		float delta = tf - t0;
		unsigned expanded_f = engine.expanded();
		unsigned generated_f = engine.generated();
		if( solved ) {
			task->mLogFile << "Plan found with cost: " << cost << std::endl;
			task->mLogFile << "Time: " << delta << std::endl;
			task->mCost = cost;
		}
		else
			task->mLogFile << "No solution found!" << std::endl;
		task->mLogFile << "Generated: " << generated_f - generated_0 << std::endl;
		task->mLogFile << "Expanded: " << expanded_f - expanded_0 << std::endl;
		expanded_0 = expanded_f;
		generated_0 = generated_f;
		runningTime += delta;
		t0 = aptk::time_used();
		task->mLogFile << "Running Time: " << runningTime << " Remaining Time: " << maxTime - runningTime << std::endl;
	} while ( solved && ( runningTime <= maxTime ) );
	task->mLogFile << "Total Running Time: " << runningTime << std::endl;
}

}
