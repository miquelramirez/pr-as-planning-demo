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
