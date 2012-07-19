#include <model/WalkTo.hxx>
#include <model/Room.hxx>
#include <model/Door.hxx>
#include <model/Agent.hxx>
#include <cassert>

namespace Application
{

WalkTo::WalkTo( Room* src, Room* dst, Door* d )
	: mSource( src ), mDestination( dst ), mDoor( d ),
	mSTRIPSAction( NULL )
{
}

WalkTo::~WalkTo()
{
}

const QString&	WalkTo::sourceName() const
{
	return mSource->name();
}

const QString&	WalkTo::destName() const
{
	return mDestination->name();
}

void	WalkTo::execute()
{
	mSource->setAgentLocation(false);
	mDestination->setAgentLocation(true);
	assert( mSTRIPSAction != NULL );
	emit executed( mSTRIPSAction->index() );
}

bool	WalkTo::checkPrecondition()
{
	return ( mDoor != NULL ? mDoor->isOpen() : true ) &&  mSource->isAgentHere() && Agent::instance().canWalk();
}

void	WalkTo::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( mSource->atFluent() != NULL );
	assert( mDestination->atFluent() != NULL );
	assert( Agent::instance().canWalkFluent() != NULL );
	if ( mDoor != NULL )
		assert( mDoor->openFluent() != NULL ); 

	std::string signature = QString( "(WalkTo %1 %2)").arg( mSource->name()).arg( mDestination->name() ).toStdString();

	mPrecFluents.push_back( Agent::instance().canWalkFluent() );
	mPrecFluents.push_back( mSource->atFluent() );
	if ( mDoor != NULL )
		mPrecFluents.push_back( mDoor->openFluent() );
	mAddFluents.push_back( mDestination->atFluent() );
	mDelFluents.push_back( mSource->atFluent() );

	aig_tk::Fluent_Vec pre, add, del;
	for (unsigned k = 0; k < mPrecFluents.size(); k++ )
		pre.push_back( mPrecFluents[k]->index() );
	for (unsigned k = 0; k < mAddFluents.size(); k++ )
		add.push_back( mAddFluents[k]->index() );
	for (unsigned k = 0; k < mDelFluents.size(); k++ )
		del.push_back( mDelFluents[k]->index() );
	assert( !pre.empty() );
	assert( !add.empty() );
	assert( !del.empty() );
	aig_tk::Conditional_Effect_Vec cEffs;

	unsigned actIndex = STRIPS_Problem::add_action( p, signature, pre, add, del, cEffs ); 
	mSTRIPSAction = p.actions()[actIndex];
	assert( !mSTRIPSAction->prec_vec().empty() );
	assert( !mSTRIPSAction->add_vec().empty());
	assert( !mSTRIPSAction->del_vec().empty() );

}

}
