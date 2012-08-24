#include <model/LieDown.hxx>
#include <model/Agent.hxx>
#include <model/Room.hxx>
#include <model/StageProp.hxx>

namespace Application
{
static StagePropActionFactoryRegistrar<LieDown>	registryObject( "Lie_Down" );

LieDown::LieDown( Agent* a, Room* r, StageProp* obj )
	: StagePropAction( a, r, obj )
{
	
}

LieDown::~LieDown()
{
}

bool	LieDown::checkPrecondition()
{
	return	roomRef().isAgentHere()
		&& agentRef().canWalk()
		&& roomRef().props().contains( &(objRef()) );	
}

void	LieDown::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( roomRef().atFluent() != NULL );
	assert( roomRef().atPropFluents()[ objRef().name() ] != NULL );
	assert( agentRef().canWalkFluent() != NULL );
	assert( objRef().getPropertyFluent( "lying" ) != NULL );
	
	std::string signature = QString( "(LieDown %1)" ).arg( objRef().name() ).toStdString();

	precs().push_back( roomRef().atFluent() );
	precs().push_back( roomRef().atPropFluents()[ objRef().name() ] );
	precs().push_back( agentRef().canWalkFluent() );
	adds().push_back( objRef().getPropertyFluent( "lying" ) );
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

void	LieDown::execute()
{
	objRef().setProperty( "lying", true );		
	agentRef().lieOn();

	emit executed( stripsAction().index() );	
}

}
