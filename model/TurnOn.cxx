#include <model/TurnOn.hxx>
#include <model/Agent.hxx>
#include <model/Room.hxx>
#include <model/StageProp.hxx>

namespace Application
{

static StagePropActionFactoryRegistrar<TurnOn>	registryObject( "Turn_On" );

TurnOn::TurnOn( Agent* a, Room* r, StageProp* obj )
	: StagePropAction( a, r, obj )
{
	
}

TurnOn::~TurnOn()
{
}

bool	TurnOn::checkPrecondition()
{
	return	roomRef().isAgentHere()
		&& roomRef().props().contains( &objRef() )
		&& objRef().getProperty( "off" );	
}

void	TurnOn::makeSTRIPSAction( STRIPS_Problem& p )
{
	assert( roomRef().atFluent() != NULL );
	assert( roomRef().atPropFluents()[ objRef().name() ] != NULL );
	assert( objRef().getPropertyFluent( "off" ) != NULL );
	assert( objRef().getPropertyFluent( "on" ) != NULL );
	
	std::string signature = QString( "(TurnOn %1)" ).arg( objRef().name() ).toStdString();

	precs().push_back( roomRef().atFluent() );
	precs().push_back( roomRef().atPropFluents()[ objRef().name() ] );
	precs().push_back( objRef().getPropertyFluent( "off" ) );
	adds().push_back( objRef().getPropertyFluent( "on" ) );
	dels().push_back( objRef().getPropertyFluent( "off" ) );

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

void	TurnOn::execute()
{
	objRef().setProperty( "off", false );
	objRef().setProperty( "on", true );		

	emit executed( stripsAction().index() );	
}

}
