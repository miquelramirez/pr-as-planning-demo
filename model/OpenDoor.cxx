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
