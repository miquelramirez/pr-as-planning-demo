#include <planning/GoalRecognizer.hxx>
#include <QString>
#include <iostream>
#include <limits>
#include <planning/Types.hxx>
#include <search/Node.hxx>
#include <planning/Single_Queue_FF_BFS.hxx>
#include <util/time.hxx>
#include <util/ext_math.hxx>
#include <planning/inference/Reachability_Test.hxx>
#include <planning/PlanningThread.hxx>

namespace Planning
{


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
	printInitAndGoal( mNotCompliantProblem );
	assert( checkReachability( mCompliantProblem ) );
	assert( checkReachability( mNotCompliantProblem ) );

	mObsCompliantCost = solve( mCompliantProblem );
	/*
	PlanningThread*	compThread = new PlanningThread( mCompliantProblem );
	QObject::connect( compThread, SIGNAL( finished() ), this, SLOT( notifyCompFinished() ) );
	PlanningThread*	notCompThread = new PlanningThread( mNotCompliantProblem );
	QObject::connect( notCompThread, SIGNAL( finished() ), this, SLOT( notifyNotCompFinished() ) );

	compThread->start();
	*/
	std::cout << "c(P[G+O])=" << mObsCompliantCost << std::endl;

	if ( !checkSolvable( mNotCompliantProblem ) )
		mNotObsCompliantCost = std::numeric_limits<aig_tk::Cost_Type>::infinity();
	else
	{
		mNotObsCompliantCost = solve( mNotCompliantProblem );
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

	assert( mObsCompliantCost != std::numeric_limits<aig_tk::Cost_Type>::infinity()
		|| mNotObsCompliantCost !=  std::numeric_limits<aig_tk::Cost_Type>::infinity() );

	if ( mObsCompliantCost == std::numeric_limits<aig_tk::Cost_Type>::infinity() )
	{
		mObsCompliantLikelihood = 0.0f;
		mNotObsCompliantLikelihood = 1.0f;
	}
	else if ( mNotObsCompliantCost == std::numeric_limits<aig_tk::Cost_Type>::infinity() )
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

void	GoalRecognizer::printState( STRIPS_Problem& prob, aig_tk::State& s )
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
	aig_tk::Reachability_Test	rtest( prob );
	return rtest.is_reachable( prob.init(), prob.goal() );
}

bool	GoalRecognizer::checkSolvable( STRIPS_Problem& prob )
{
	aig_tk::Reachability_Test	rtest( prob );
	
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

float	GoalRecognizer::solve( STRIPS_Problem& prob )
{
	float t0, tf;

	t0 = time_used();
	aig_tk::Node* n0 = aig_tk::Node::root( prob );
	std::vector<aig_tk::Node*> plan;
	std::vector<Action*>	best_plan;
	aig_tk::Single_Queue_FF_BFS engine;
	engine.init( prob, n0 );
	engine.start();
	float maxTime = 0.1f;
	float runningTime = 0.0f;	

	aig_tk::Cost_Type best = std::numeric_limits<aig_tk::Cost_Type>::infinity();
	bool solved;
	do
	{
		plan.clear();
		engine.set_budget( maxTime - runningTime );
		solved = engine.findSolution(plan);
		tf = time_used();
		std::cout << "Plan computed:"; report_interval( t0, tf, std::cout ); std::cout << std::endl;
	
		if( solved )
		{
			aig_tk::Cost_Type plan_cost = 0.0f;
			aig_tk::State* s0 = n0->s();
			std::vector< aig_tk::State* > trace;
			aig_tk::State* current = s0;
			for ( unsigned k = 0; k < plan.size(); k++ )
			{
				Action* a = plan[k]->op();
				assert(  current->entails(a->prec_vec()) );
				plan_cost = aig_tk::add( plan_cost, a->cost() );
				current = current->progress_through( *a );
				trace.push_back( current );
			}
			for ( unsigned k = 0; k < trace.size(); k++ )
				delete trace[k];
			best = plan_cost;
			best_plan.clear();
			for ( unsigned k = 0; k < plan.size(); k++ )
				best_plan.push_back( plan[k]->op() );
		}
		runningTime += (tf - t0);
		t0 = time_used();
		std::cout << "Running Time: " << runningTime << std::endl;
	} while ( solved && ( runningTime <= maxTime ) );
	std::cout << "Total Running Time: " << runningTime << std::endl;
	printPlan( prob, best_plan );
	return best;

}


}
