#include <model/Drop.hxx>
#include <model/Room.hxx>
#include <model/Agent.hxx>
#include <model/Item.hxx>

namespace Application
{

Drop::Drop( Agent* a, Room* r, Item* i )
	: mAgent(a), mRoom(r), mItem(i), mSTRIPSAction(NULL)
{
}

Drop::~Drop()
{
}

const QString&	Drop::roomName() const
{
	return mRoom->name();
}

const QString&	Drop::itemName() const
{
	return mItem->name();
}

void	Drop::execute()
{
	mAgent->removeItem( mItem );
	mRoom->addItem( mItem );
	assert( mSTRIPSAction != NULL );
	emit executed( mSTRIPSAction->index() );
}

bool	Drop::checkPrecondition()
{
	return mAgent->inventory().contains( mItem );
}

void	Drop::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( mRoom->atFluent() != NULL );
	assert( mRoom->atItemFluents()[ mItem->name() ] != NULL );
	assert( mAgent->carryingFluents()[ mItem->name() ] );
	
	std::string signature = QString( "(Drop %1 %2)" )
				.arg( itemName() ).arg( mRoom->name() ).toStdString();

	mPrecFluents.push_back( mRoom->atFluent() );
	mPrecFluents.push_back( mAgent->carryingFluents()[ mItem->name() ] );
	mAddFluents.push_back( mRoom->atItemFluents()[ mItem->name() ] );
	mDelFluents.push_back( mAgent->carryingFluents()[ mItem->name() ] );

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
	assert( !mSTRIPSAction->add_vec().empty() );
	assert( !mSTRIPSAction->del_vec().empty() );
	
}

}
