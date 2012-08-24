#include <model/SitDown.hxx>
#include <model/Agent.hxx>
#include <model/Room.hxx>
#include <model/StageProp.hxx>

namespace Application
{
static StagePropActionFactoryRegistrar<SitDown>	registryObject("Sit_Down");

SitDown::SitDown( Agent* a, Room* r, StageProp* obj )
	: StagePropAction( a, r, obj )
{
	
}

SitDown::~SitDown()
{
}

bool	SitDown::checkPrecondition()
{
	return	roomRef().isAgentHere()
		&& agentRef().canWalk()
		&& roomRef().props().contains( &(objRef()) );
}

void	SitDown::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( roomRef().atFluent() != NULL );
	assert( roomRef().atPropFluents()[ objRef().name() ] != NULL );
	assert( agentRef().canWalkFluent() != NULL );
	assert( objRef().getPropertyFluent( "sitting" ) != NULL );

	std::string signature = QString( "(SitDown %1)" ).arg( objRef().name() ).toStdString();

	precs().push_back( roomRef().atFluent() );
	precs().push_back( roomRef().atPropFluents()[ objRef().name() ] );
	precs().push_back( agentRef().canWalkFluent() );
	adds().push_back( objRef().getPropertyFluent( "sitting" ) );
	dels().push_back( agentRef().canWalkFluent() );

	aptk::Fluent_Vec pre, add, del;
	for (unsigned k = 0; k < precs().size(); k++ )
		pre.push_back( precs()[k]->index() );
	for (unsigned k = 0; k < adds().size(); k++ )
		add.push_back( adds()[k]->index() );
	for (unsigned k = 0; k < dels().size(); k++ )
		del.push_back( dels()[k]->index() );

	assert( !pre.empty() );
	assert( !add.empty() );
	assert( !del.empty() );
	aptk::Conditional_Effect_Vec cEffs;

	unsigned actIndex = STRIPS_Problem::add_action( p, signature, pre, add, del, cEffs ); 
	setSTRIPSAction( p.actions()[actIndex] );

	assert( !mSTRIPSAction->prec_vec().empty() );
	assert( !mSTRIPSAction->add_vec().empty() );
	assert( !mSTRIPSAction->del_vec().empty() );
}

void	SitDown::execute()
{
	objRef().setProperty( "sitting", true );	
	agentRef().sitDown();	

	emit executed( stripsAction().index() );	
}

}
