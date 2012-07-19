#include <model/TurnOff.hxx>
#include <model/Agent.hxx>
#include <model/Room.hxx>
#include <model/StageProp.hxx>

namespace Application
{
static StagePropActionFactoryRegistrar<TurnOff>	registryObject( "Turn_Off" );

TurnOff::TurnOff( Agent* a, Room* r, StageProp* obj )
	: StagePropAction( a, r, obj )
{
	
}

TurnOff::~TurnOff()
{
}

bool	TurnOff::checkPrecondition()
{
	return	roomRef().isAgentHere()
		&& roomRef().props().contains( &objRef() )
		&& objRef().getProperty( "on" );	
}

void	TurnOff::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( roomRef().atFluent() != NULL );
	assert( roomRef().atPropFluents()[ objRef().name() ] != NULL );
	assert( objRef().getPropertyFluent( "off" ) != NULL );
	assert( objRef().getPropertyFluent( "on" ) != NULL );
	
	std::string signature = QString( "(TurnOn %1)" ).arg( objRef().name() ).toStdString();

	precs().push_back( roomRef().atFluent() );
	precs().push_back( roomRef().atPropFluents()[ objRef().name() ] );
	precs().push_back( objRef().getPropertyFluent( "on" ) );
	adds().push_back( objRef().getPropertyFluent( "off" ) );
	dels().push_back( objRef().getPropertyFluent( "on" ) );

	aig_tk::Fluent_Vec pre, add, del;
	for (unsigned k = 0; k < precs().size(); k++ )
		pre.push_back( precs()[k]->index() );
	for (unsigned k = 0; k < adds().size(); k++ )
		add.push_back( adds()[k]->index() );
	for (unsigned k = 0; k < dels().size(); k++ )
		del.push_back( dels()[k]->index() );

	assert( !pre.empty() );
	assert( !add.empty() );
	assert( !del.empty() );
	aig_tk::Conditional_Effect_Vec cEffs;

	unsigned actIndex = STRIPS_Problem::add_action( p, signature, pre, add, del, cEffs ); 
	setSTRIPSAction( p.actions()[actIndex] );

	assert( !mSTRIPSAction->prec_vec().empty() );
	assert( !mSTRIPSAction->add_vec().empty() );
	assert( !mSTRIPSAction->del_vec().empty() );
}

void	TurnOff::execute()
{
	objRef().setProperty( "on", false );
	objRef().setProperty( "off", true );		

	emit executed( stripsAction().index() );	
}

}
