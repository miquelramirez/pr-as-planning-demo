#include <model/Agent.hxx>
#include <model/Item.hxx>
#include <model/ItemManager.hxx>
#include <cassert>

namespace Application
{

Agent::Agent()
	: mCanWalk( true ), mCanWalkFluent( NULL ) 
{
}

Agent::~Agent()
{
}

Agent&	Agent::instance()
{
	static Agent	theAgent;
	return theAgent;
}

void	Agent::sitDown()
{
	mCanWalk = false;
}

void	Agent::lieOn()
{
	mCanWalk = false;
}

void	Agent::getUp()
{
	mCanWalk = true;	
}

void	Agent::addItem( Item* i )
{
	inventory().push_back( i );
}

void	Agent::removeItem( Item* i )
{
	inventory().removeOne( i );
}

void	Agent::makeSTRIPSFluents( STRIPS_Problem& domain )
{
	unsigned index = STRIPS_Problem::add_fluent( domain, QString( "(can_walk)").toStdString() );
	mCanWalkFluent = domain.fluents()[index];

	QList< Item* >& itemList = ItemManager::instance().items();

	for ( int i = 0; i < itemList.size(); i++ )
	{
		index = STRIPS_Problem::add_fluent( domain, QString( "(carrying %1)" ).arg( itemList[i]->name() ).toStdString() );
		mCarryingFluents[ itemList[i]->name() ] = domain.fluents()[index];
	}
}

void	Agent::evalSTRIPSFluents( aig_tk::Fluent_Vec& eval )
{
	if ( canWalk() )
		eval.push_back( mCanWalkFluent->index() );
	for ( int k = 0; k < mInventory.size(); k++ )
		eval.push_back( carryingFluents()[ mInventory[k]->name() ]->index() );
}


}
