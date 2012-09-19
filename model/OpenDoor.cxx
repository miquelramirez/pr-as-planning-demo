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

#include <model/OpenDoor.hxx>
#include <model/Door.hxx>
#include <model/Room.hxx>

namespace Application
{

OpenDoor::OpenDoor( Door* d, Room* r )
	: mDoor( d ), mRoom( r ), mSTRIPSAction( NULL )
{
}

OpenDoor::~OpenDoor()
{
}

const QString& OpenDoor::doorName() const
{
	return mDoor->name();
}

const QString&	OpenDoor::roomName() const
{
	return mRoom->name();
}

void	OpenDoor::execute()
{
	mDoor->open();
	assert( mSTRIPSAction != NULL );
	emit executed( mSTRIPSAction->index() );
}

bool	OpenDoor::checkPrecondition()
{
	return !mDoor->isOpen() && mRoom->isAgentHere();
}

void	OpenDoor::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( mDoor->openFluent() != NULL );
	assert( mDoor->closedFluent() != NULL );
	assert( mRoom->atFluent() != NULL );
	
	std::string signature = QString( "(Open %1 %2)" ).arg( roomName() ).arg( doorName() ).toStdString();

	mPrecFluents.push_back( mDoor->closedFluent() );
	mPrecFluents.push_back( mRoom->atFluent() );
	mAddFluents.push_back( mDoor->openFluent() );
	mDelFluents.push_back( mDoor->closedFluent() );

	aptk::Fluent_Vec pre, add, del;
	for (unsigned k = 0; k < mPrecFluents.size(); k++ )
		pre.push_back( mPrecFluents[k]->index() );
	for (unsigned k = 0; k < mAddFluents.size(); k++ )
		add.push_back( mAddFluents[k]->index() );
	for (unsigned k = 0; k < mDelFluents.size(); k++ )
		del.push_back( mDelFluents[k]->index() );
	assert( !pre.empty() );
	assert( !add.empty() );
	assert( !del.empty() );
	aptk::Conditional_Effect_Vec cEffs;

	unsigned actIndex = STRIPS_Problem::add_action( p, signature, pre, add, del, cEffs ); 
	mSTRIPSAction = p.actions()[actIndex];
	assert( !mSTRIPSAction->prec_vec().empty() );
	assert( !mSTRIPSAction->add_vec().empty());
	assert( !mSTRIPSAction->del_vec().empty() );
}

} 
