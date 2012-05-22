#include <planning/Observer.hxx>
#include <planning/Action.hxx>
#include <cstdlib>
#include <iostream>
#include <widgets/GoalView.hxx>
#include <planning/GoalRecognizer.hxx>

namespace Planning
{

Observer::Observer( STRIPS_Problem& domain )
	: mDomain ( domain ), mStarted( false ), mObsLevel( 1.0f )
{
}

Observer::~Observer()
{
}

void	Observer::start()
{
	mStarted = true;
	mObservations.clear();
	if ( !mHypGoals.empty() )
	{
		float goalProb = 1.0f / (float)mHypGoals.size();
		for ( std::list< Goal* >::iterator it = mHypGoals.begin();
			it != mHypGoals.end(); it++ )
			(*it)->setProbability( goalProb );
	}
	std::cout << "Observer: observing" << std::endl;
	emit requestInitialStateUpdate();	
}

void	Observer::updateInitialState( aig_tk::Fluent_Vec& eval )
{
	mInitialState = eval;
	
	std::cout << "Observer: initial state" << std::endl;
	for ( unsigned k = 0; k < mInitialState.size(); k++ )
		std::cout << "\t" << mDomain.fluents()[mInitialState[k]]->signature() << std::endl;
}

void	Observer::stop()
{
	mStarted = false;
	std::cout << "Observer: not observing" << std::endl;
}

void	Observer::setObsLevel( float v )
{
	mObsLevel = v;
	std::cout << "Observer: observation level set to " << v << std::endl;
}

void	Observer::actionExecuted( unsigned index )
{
	if ( !mStarted ) return;

	float rX = (float)rand()/(float)RAND_MAX;
	
	if ( rX >= obsLevel() ) return;
	
	aig_tk::Action*	obs = mDomain.actions()[index];
	
	mObservations.push_back( obs );
	std::cout << "Observed " << obs->signature() << std::endl;
	std::cout << "Observed so far " << std::endl;
	for ( unsigned k = 0; k < mObservations.size(); k++ )
	{
		std::cout << k+1 << ". " << mObservations[k]->signature() << std::endl;
	}
	if ( mHypGoals.empty() ) return;

	std::vector<GoalRecognizer*> recognizers;
	for ( std::list< Goal* >::iterator it = mHypGoals.begin();
		it != mHypGoals.end(); it++ )
		recognizers.push_back( new GoalRecognizer( mDomain, **it, mObservations, mInitialState ) ); 

	for ( unsigned k = 0; k < recognizers.size(); k++ )
		recognizers[k]->evaluateLikelihoods();

	float norm = 0.0f;
	for ( unsigned k = 0; k < recognizers.size(); k++ )
		norm += recognizers[k]->obsCompliantLikelihood();
	unsigned k = 0;
	for ( std::list< Goal* >::iterator it = mHypGoals.begin();
		it != mHypGoals.end(); it++ )
	{
		(*it)->setProbability( recognizers[k]->obsCompliantLikelihood() / norm );
		k++;
	}

	for ( unsigned k = 0; k < recognizers.size(); k++ )
		delete recognizers[k];
	
}

void	Observer::addNewGoal( QString label, QList< QString >& fluents  )
{
	aig_tk::Fluent_Vec fluentVec;
	std::cout << "Creating new hypothetic goal" << std::endl;
	
	for ( int i = 0; i < fluents.size(); i++ )
	{
		unsigned f = mDomain.getFluentIndex( fluents[i].toStdString() );
		std::cout << "Fluent in goal: " << fluents[i].toStdString() << " " << f << std::endl;
		fluentVec.push_back( f );
	}	

	Goal*	g = new Goal( mDomain, fluentVec );
	g->setName( label );
	mHypGoals.push_back( g );
	
	UI::GoalView* gView = new UI::GoalView( g->name() );
	gView->makeTooltip( g->fluentNames() );
	QObject::connect( gView, SIGNAL( removeGoal( QString ) ),
				this, SLOT( removeGoal( QString ) ) );

	QObject::connect( g, SIGNAL( probabilityChanged( float ) ),
				gView, SLOT( setProbability( float ) ) );	
	emit goalViewCreated( gView );
}

void	Observer::removeGoal( QString label )
{
	std::list< Goal* >::iterator it;
	for ( it = mHypGoals.begin();
		it != mHypGoals.end(); it++ )
	{
		if ( (*it)->name() == label )
			break;	
	}
	Goal* removed = *it;
	mHypGoals.erase( it );
	delete removed;
}

void	Observer::onHypGoalsInfoRequested()
{
	std::cout << "Hyp Goals info requested" << std::endl;
	QList< QString > fluents;
	QList< QString > hypGoalNames;

	for ( unsigned k = 0; k < mDomain.fluents().size(); k++ )
		fluents.push_back( QString( mDomain.fluents()[k]->signature().c_str() ) );
	for ( std::list< Goal* >::iterator it = mHypGoals.begin();
		it != mHypGoals.end(); it++ )
		hypGoalNames.push_back( (*it)->name() );

	emit sendHypGoalsInfo( fluents, hypGoalNames );
}

}
