#include <model/SimHome.hxx>
#include <model/Home.hxx>
#include <cstdlib>
#include <iostream>
#include <QApplication>
#include <widgets/RoomView.hxx>
#include <model/Room.hxx>
#include <model/Door.hxx>
#include <model/Agent.hxx>
#include <planning/Observer.hxx>

namespace Application
{

SimHome::SimHome()
	: mHome( NULL )
{
	mObserver = new Planning::Observer( mDomain );
	QObject::connect( this, SIGNAL(  updateInitialState( aig_tk::Fluent_Vec& ) ),
			mObserver, SLOT( updateInitialState( aig_tk::Fluent_Vec& ) ) );
	QObject::connect( mObserver, SIGNAL( requestInitialStateUpdate() ),
			this, SLOT( initialStateUpdateRequested() ) );
}

SimHome::~SimHome()
{
	delete mObserver;
}

void	SimHome::quit()
{
	std::cout << "Quitting Sim Home" << std::endl;
	std::exit(0);
}

void	SimHome::loadHome( const QString& homePath )
{
	std::cout << "Loading Home from xml document: " << homePath.toStdString() << std::endl;
	if ( mHome ) 
		delete	mHome;
	mHome = new Home;
	if ( Home::load( homePath, mHome ) )
		emit onHomeLoaded( mHome->floorPlan() );
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
	{
		UI::RoomView* v =  new UI::RoomView( mHome->rooms()[k], NULL );
		QObject::connect( mHome->rooms()[k], SIGNAL( showAgent() ), v, SLOT( showAgent() ) );
		QObject::connect( mHome->rooms()[k], SIGNAL( hideAgent() ), v, SLOT( hideAgent() ) );
		emit makeRoom( v );
	}
	
	Agent::instance().makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
		mHome->rooms()[k]->makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->doors().size(); k++ )
		mHome->doors()[k]->makeSTRIPSFluents(mDomain);
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
		mHome->rooms()[k]->makeSTRIPSActions(mDomain, mObserver );
	for ( unsigned k = 0; k < mHome->doors().size(); k++ )
		mHome->doors()[k]->makeSTRIPSActions(mDomain, mObserver );

	std::cout << "STRIPS fluents: " << mDomain.fluents().size() << std::endl;
	std::cout << "STRIPS actions: " << mDomain.actions().size() << std::endl;
}

void	SimHome::initialStateUpdateRequested()
{
	aig_tk::Fluent_Vec evalResult;
	Agent::instance().evalSTRIPSFluents( evalResult );	
	for ( unsigned k = 0; k < mHome->rooms().size(); k++ )
		mHome->rooms()[k]->evalSTRIPSFluents( evalResult );
	for ( unsigned k = 0; k < mHome->doors().size(); k++ )
		mHome->doors()[k]->evalSTRIPSFluents( evalResult );
	emit updateInitialState( evalResult );
}

}

