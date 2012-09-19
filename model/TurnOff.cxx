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
	
	std::string signature = QString( "(TurnOff %1)" ).arg( objRef().name() ).toStdString();

	precs().push_back( roomRef().atFluent() );
	precs().push_back( roomRef().atPropFluents()[ objRef().name() ] );
	precs().push_back( objRef().getPropertyFluent( "on" ) );
	adds().push_back( objRef().getPropertyFluent( "off" ) );
	dels().push_back( objRef().getPropertyFluent( "on" ) );

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

void	TurnOff::execute()
{
	objRef().setProperty( "on", false );
	objRef().setProperty( "off", true );		

	emit executed( stripsAction().index() );	
}

}
