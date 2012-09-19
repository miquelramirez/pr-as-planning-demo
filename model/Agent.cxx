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

void	Agent::evalSTRIPSFluents( aptk::Fluent_Vec& eval )
{
	if ( canWalk() )
		eval.push_back( mCanWalkFluent->index() );
	for ( int k = 0; k < mInventory.size(); k++ )
		eval.push_back( carryingFluents()[ mInventory[k]->name() ]->index() );
}


}
