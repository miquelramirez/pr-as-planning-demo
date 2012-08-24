#include <planning/GoalRecognizer.hxx>
#include <QString>
#include <iostream>
#include <sstream>
#include <limits>
#include <types.hxx>
#include <aptk/resources_control.hxx>
#include <aptk/ext_math.hxx>
#include <reachability.hxx>
//#include <planning/PlanningThread.hxx>

#include <strips_state.hxx>
#include <fwd_search_prob.hxx>
#include <h_1.hxx>
#include <rp_heuristic.hxx>
#include <simple_landmarks.hxx>
#include <aptk/open_list.hxx>
#include <aptk/at_rwbfs_dq_mh.hxx>

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

GoalRecognizer::GoalRecognizer(  STRIPS_Problem& p, Goal& g, Action_Ptr_Vec& obs, Fluent_Vec& init )
	: mBeta( 1.0f ), mHypGoal( g ), mObsSequence(obs), mInitialState( init ), mOriginalProblem( p ),
	mObsCompliantLikelihood( 0.0f ), mNotObsCompliantLikelihood( 0.0f ), mObsCompliantCost( 0.0f ),
	mNotObsCompliantCost( 0.0f ), mFinishedComp( false ), mFinishedNotComp( false )
{
	copyFluents();
	compileObservations();
	setInitialStates(mCompliantProblem, mNotObsFluentsComp);
	setInitialStates(mNotCompliantProblem, mNotObsFluentsNotComp);
	setGoals(mCompliantProblem, mObsFluentsComp);
	setGoals(mNotCompliantProblem, mNotObsFluentsNotComp);
	mCompliantProblem.make_action_tables();
	mNotCompliantProblem.make_action_tables();

	std::stringstream comp_sstr;
	comp_sstr << mHypGoal.name().toStdString();
	comp_sstr << ".comp.log";
	mLogFileComp = comp_sstr.str();
	
	std::stringstream not_comp_sstr;
	not_comp_sstr << mHypGoal.name().toStdString();
	not_comp_sstr << ".not-comp.log";
	mLogFileNotComp = not_comp_sstr.str();
}

GoalRecognizer::~GoalRecognizer()
{
}

bool	GoalRecognizer::finished()
{
	return mFinishedNotComp && mFinishedComp;
}

void	GoalRecognizer::notifyCompFinished()
{
	mFinishedComp = true;
}

void	GoalRecognizer::notifyNotCompFinished()
{
	mFinishedNotComp = true;
}

void	GoalRecognizer::copyFluents()
{
	for ( unsigned k = 0; k < mOriginalProblem.fluents().size(); k++ )
	{
		Fluent* f = mOriginalProblem.fluents()[k];
		STRIPS_Problem::add_fluent( mCompliantProblem, f->signature() );
		STRIPS_Problem::add_fluent( mNotCompliantProblem, f->signature() );
	}
}

void	GoalRecognizer::createObsFluents(STRIPS_Problem& prob, Fluent_Ptr_Vec& obsFluents, Fluent_Ptr_Vec& notObsFluents )
{
	for ( unsigned k = 0; k < mObsSequence.size(); k++ )
	{
		std::string signature = QString( "(obs_%1)" ).arg( k ).toStdString();
		std::string signature_not = QString( "(not_obs_%1)" ).arg(k).toStdString();
		unsigned index = STRIPS_Problem::add_fluent( prob, signature );
		obsFluents.push_back( prob.fluents()[index] );
		index = STRIPS_Problem::add_fluent( prob, signature_not );
		notObsFluents.push_back( prob.fluents()[index] );
	}
}

void	GoalRecognizer::createActions(STRIPS_Problem& prob, Fluent_Ptr_Vec& obsFluents, Fluent_Ptr_Vec& notObsFluents)
{
	for ( unsigned k = 0; k < mOriginalProblem.actions().size(); k++ )
	{
		Action* actOrig = mOriginalProblem.actions()[k];
		
		
		Fluent_Vec precs = actOrig->prec_vec();
		Fluent_Vec adds = actOrig->add_vec();
		Fluent_Vec dels = actOrig->del_vec();
		Conditional_Effect_Vec ceffs;
		for ( unsigned k = 0; k < actOrig->ceff_vec().size(); k++ )
		{
			Fluent_Vec cprecs = actOrig->ceff_vec()[k]->prec_vec();
			Fluent_Vec cadds =  actOrig->ceff_vec()[k]->add_vec();
			Fluent_Vec cdels = actOrig->ceff_vec()[k]->del_vec();
			Conditional_Effect* eff = new Conditional_Effect( prob );
			eff->define( cprecs, cadds, cdels );
			ceffs.push_back( eff );
		}		

		std::vector<unsigned>	obs_hit;
		for ( unsigned k = 0; k < mObsSequence.size(); k++ )
		{
			if ( actOrig->index() == mObsSequence[k]->index() )
				obs_hit.push_back( k );
		}
 
		// Create conditional effects		
		for ( unsigned k = 0; k < obs_hit.size(); k++ )
		{
			int obs_index = obs_hit[k];
			int prev_index = obs_index-1;
			Fluent_Vec cprec, cadd, cdel;
			if ( prev_index >= 0 )
				cprec.push_back( obsFluents[prev_index]->index() ); 	
			cadd.push_back( obsFluents[obs_index]->index() );
			cdel.push_back( notObsFluents[obs_index]->index() );

			Conditional_Effect* eff = new Conditional_Effect(prob);
			eff->define( cprec, cadd, cdel );
			ceffs.push_back( eff );
		}


		STRIPS_Problem::add_action( 	prob, actOrig->signature(),
						precs, adds, dels, ceffs );

	}
}

void	GoalRecognizer::compileObservations()
{
	createObsFluents(mCompliantProblem, mObsFluentsComp, mNotObsFluentsComp);
	createObsFluents(mNotCompliantProblem, mObsFluentsNotComp, mNotObsFluentsNotComp);
	createActions(mCompliantProblem, mObsFluentsComp, mNotObsFluentsComp );
	createActions(mNotCompliantProblem, mObsFluentsNotComp, mNotObsFluentsNotComp );
}

void	GoalRecognizer::setInitialStates(STRIPS_Problem& p, Fluent_Ptr_Vec& notObsFluents )
{
	Fluent_Vec v = mInitialState;
	for ( unsigned k = 0; k < notObsFluents.size(); k++ )
		v.push_back( notObsFluents[k]->index() );
	
	STRIPS_Problem::set_init(p, v );
}

void	GoalRecognizer::setGoals(STRIPS_Problem& p, Fluent_Ptr_Vec& obsFluents)
{
	Fluent_Vec v = mHypGoal.fluents();
	v.push_back( obsFluents.back()->index() );
	STRIPS_Problem::set_goal( p, v );
}

void	GoalRecognizer::printInitAndGoal( STRIPS_Problem& p )
{
	std::cout << "Initial State:" << std::endl;
	for ( unsigned k = 0; k < p.init().size(); k++ )
		std::cout << p.fluents()[p.init()[k]]->signature() << std::endl;

	std::cout << "Goal:" << std::endl;
	for ( unsigned k = 0; k < p.goal().size(); k++ )
		std::cout << p.fluents()[p.goal()[k]]->signature() << std::endl;	
}

void	GoalRecognizer::evaluateLikelihoods()
{
	std::cout << "Evaluating Goal: " << mHypGoal.name().toStdString() << std::endl;
	printInitAndGoal( mCompliantProblem );
	printActions( mCompliantProblem );
	printInitAndGoal( mNotCompliantProblem );
	assert( checkReachability( mCompliantProblem ) );
	assert( checkReachability( mNotCompliantProblem ) );

	mObsCompliantCost = solve( mCompliantProblem, mLogFileComp );
	/*
	PlanningThread*	compThread = new PlanningThread( mCompliantProblem );
	QObject::connect( compThread, SIGNAL( finished() ), this, SLOT( notifyCompFinished() ) );
	PlanningThread*	notCompThread = new PlanningThread( mNotCompliantProblem );
	QObject::connect( notCompThread, SIGNAL( finished() ), this, SLOT( notifyNotCompFinished() ) );

	compThread->start();
	*/
	std::cout << "c(P[G+O])=" << mObsCompliantCost << std::endl;

	if ( !checkSolvable( mNotCompliantProblem ) )
		mNotObsCompliantCost = infty;
	else
	{
		mNotObsCompliantCost = solve( mNotCompliantProblem, mLogFileNotComp );
		//notCompThread->start();
	}
	std::cout << "c(P[G+\\overline{O}])=" << mNotObsCompliantCost << std::endl;

	/*
	while (!finished())
		usleep( 100000 );
	mObsCompliantCost = compThread->result();
	mNotObsCompliantCost = compThread->result(); 
	delete compThread;
	delete notCompThread;
	*/

	assert( mObsCompliantCost != std::numeric_limits<aptk::Cost_Type>::infinity()
		|| mNotObsCompliantCost !=  std::numeric_limits<aptk::Cost_Type>::infinity() );

	if ( mObsCompliantCost == std::numeric_limits<aptk::Cost_Type>::infinity() )
	{
		mObsCompliantLikelihood = 0.0f;
		mNotObsCompliantLikelihood = 1.0f;
	}
	else if ( mNotObsCompliantCost == std::numeric_limits<aptk::Cost_Type>::infinity() )
	{
		mObsCompliantLikelihood = 1.0f;
		mNotObsCompliantLikelihood = 0.0f;
	}
	else
	{
		mObsCompliantLikelihood = 1.0f / ( 1.0f + expf( -mBeta *( mNotObsCompliantCost - mObsCompliantCost ) ) );
		mNotObsCompliantLikelihood = 1.0 - mObsCompliantLikelihood;
	}

	std::cout << "P(O|G)=" << mObsCompliantLikelihood << std::endl;
	std::cout << "P(\\overline{O}|G)=" << mNotObsCompliantLikelihood << std::endl;
}

void	GoalRecognizer::printFluentVec( STRIPS_Problem& prob, Fluent_Vec& v )
{
	for ( unsigned k = 0; k < v.size(); k++ )
	{	
		unsigned p = v[k];
		std::cout << prob.fluents()[p]->signature();
		if ( k < v.size()-1 ) std::cout << ", ";
	}	
}

void	GoalRecognizer::printState( STRIPS_Problem& prob, aptk::State& s )
{
	printFluentVec( prob, s.fluent_vec() );
}

void	GoalRecognizer::printAction( STRIPS_Problem& prob, Action& a )
{
	std::cout << "Action " << a.signature() << std::endl;
	std::cout << "\tPre(a) = {";
	printFluentVec( prob, a.prec_vec() );
	std::cout << "}" << std::endl;
	std::cout << "\tAdd(a) = {";
	printFluentVec( prob, a.add_vec() );
	std::cout << "}" << std::endl;
	std::cout << "\tDel(a) = {";
	printFluentVec( prob, a.del_vec() );
	std::cout << "}" << std::endl;
	std::cout << "\tConditional Effects:" << std::endl;
	for ( unsigned l = 0; l < a.ceff_vec().size(); l++ )
	{
		std::cout << "\t\tcond_eff " << l+1 << ":" << std::endl;
		std::cout << "\t\tPre(cond_eff) = {";
		printFluentVec( prob, a.ceff_vec()[l]->prec_vec() );
		std::cout << "}" << std::endl;
		std::cout << "\t\tAdd(cond_eff) = {";
		printFluentVec( prob, a.ceff_vec()[l]->add_vec() );
		std::cout << "}" << std::endl;
		std::cout << "\t\tDel(cond_eff) = {";
		printFluentVec( prob, a.ceff_vec()[l]->del_vec() );
		std::cout << "}" << std::endl;
	}
}

void	GoalRecognizer::printActions( STRIPS_Problem& prob )
{
	std::cout << "Actions" << std::endl;
	for ( unsigned k = 0; k < prob.actions().size(); k++ )
		printAction( prob, *prob.actions()[k] );
}

void	GoalRecognizer::printPlan( STRIPS_Problem& prob, std::vector<Action*>& plan )
{
	for ( unsigned k = 0; k < plan.size(); k++ )
	{
		std::cout << k+1 << " - ";
		printAction( prob, *plan[k] );
	}
}

bool	GoalRecognizer::checkReachability( STRIPS_Problem& prob )
{
	Reachability_Test	rtest( prob );
	return rtest.is_reachable( prob.init(), prob.goal() );
}

bool	GoalRecognizer::checkSolvable( STRIPS_Problem& prob )
{
	Reachability_Test	rtest( prob );
	
	if (rtest.is_reachable( mInitialState, mHypGoal.fluents(), mObsSequence.back()->index() ) )
		return true;
	for ( unsigned i = 0; i < mObsSequence.size()-1; i++ )
	{
	
		Fluent_Vec& prec_i = prob.actions()[mObsSequence[i+1]->index()]->prec_vec();

		if( rtest.is_reachable( mInitialState, prec_i, mObsSequence[i]->index() ) )
			return true;
	}
	return false;
}

float	GoalRecognizer::solve( STRIPS_Problem& plan_prob, std::string logfile )
{
	Fwd_Search_Problem		search_prob( &plan_prob );
	Anytime_RWBFS_H_Add_Rp_Fwd	engine( search_prob );
	
	engine.set_schedule( 10, 5, 1 );

	
	std::ofstream out( logfile.c_str() );

	engine.set_budget( 0.1f );
	engine.start();

	std::vector< aptk::Action_Idx > plan;
	float				cost;

	float ref = aptk::time_used();
	float t0 = aptk::time_used();

	unsigned expanded_0 = engine.expanded();
	unsigned generated_0 = engine.generated();

	while ( engine.find_solution( cost, plan ) ) {
		out << "Plan found with cost: " << cost << std::endl;
		for ( unsigned k = 0; k < plan.size(); k++ ) {
			out << k+1 << ". ";
			const Action& a = *(plan_prob.actions()[ plan[k] ]);
			out << a.signature();
			out << std::endl;
		}
		float tf = aptk::time_used();
		unsigned expanded_f = engine.expanded();
		unsigned generated_f = engine.generated();
		out << "Time: " << tf - t0 << std::endl;
		out << "Generated: " << generated_f - generated_0 << std::endl;
		out << "Expanded: " << expanded_f - expanded_0 << std::endl;
		t0 = tf;
		expanded_0 = expanded_f;
		generated_0 = generated_f;
		plan.clear();
	}
	float total_time = aptk::time_used() - ref;
	out << "Total time: " << total_time << std::endl;
	out << "Nodes generated during search: " << engine.generated() << std::endl;
	out << "Nodes expanded during search: " << engine.expanded() << std::endl;
	out << "Nodes pruned by bound: " << engine.pruned_by_bound() << std::endl;
	out << "Dead-end nodes: " << engine.dead_ends() << std::endl;
	out << "Nodes in OPEN replaced: " << engine.open_repl() << std::endl;

	out.close();

	return cost;

}


}
