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

#include <StagePropAction.hxx>
#include <Agent.hxx>
#include <Room.hxx>
#include <StageProp.hxx>
#include <iostream>

namespace Application
{

StagePropAction::StagePropAction( Agent* a, Room* r, StageProp* obj )
	: mAgent(a), mRoom( r ), mObject( obj )
{
}

StagePropAction::~StagePropAction()
{
}

Agent&	StagePropAction::agentRef()
{
	return *mAgent;
}

Room&	StagePropAction::roomRef()
{
	return *mRoom;
}

StageProp&	StagePropAction::objRef()
{
	return *mObject;
}

QString	StagePropAction::roomName() const
{
	return mRoom->name();
}

QString StagePropAction::objName() const
{
	return mObject->name();
}

void	StagePropAction::setSTRIPSAction( Action* a )
{
	mSTRIPSAction = a;
}

Action&	StagePropAction::stripsAction() 
{
	return *mSTRIPSAction;
}

StagePropActionCreator::~StagePropActionCreator()
{
}

StagePropActionFactory::StagePropActionFactory()
{
}

StagePropActionFactory::~StagePropActionFactory()
{
}

StagePropActionFactory&	StagePropActionFactory::instance()
{
	static StagePropActionFactory	theInstance;
	return theInstance;
}

StagePropAction*	StagePropActionFactory::produce( QString prodKey, Agent* a, Room* r, StageProp* obj )
{

	StagePropActionCreator* creator = mProductCreators[prodKey];
	if ( creator == NULL )
	{
		std::cerr << "StagePropActionFactory: Can't produce unknown product " << prodKey.toStdString() << std::endl;
		return NULL; 
	}

	return (*creator)(a, r, obj);
}

void	StagePropActionFactory::registerProduct( QString prodKey, StagePropActionCreator* creator )
{
	mProductCreators[prodKey] = creator;
}

}
