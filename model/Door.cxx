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

#include <model/Door.hxx>
#include <planning/Observer.hxx>
#include <model/Room.hxx>
#include <model/OpenDoor.hxx>
#include <model/CloseDoor.hxx>
#include <QMessageBox>
#include <QApplication>

namespace Application
{

Door::Door()
	: mName(""), mOpen(false), mOpenFluent( NULL ), mClosedFluent( NULL )
{
}

Door::~Door()
{
}

bool	Door::load( const QDomElement& root, Door* obj )
{
	if (!root.hasAttribute( "name" ) )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Document is not well formed "
			"'name' attribute missing in <Door> element."));
        	 return false;
	}
	obj->mName = root.attribute("name");
	
	if (!root.hasAttribute( "open" ) )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Document is not well formed "
			"'open' attribute missing in <Door> element."));
        	 return false;
		
	}
	obj->mOpen = ( root.attribute("open") == "yes" ? true : false );

	return true;
}

void	Door::addRoom( Room* r )
{
	openActions().push_back( new OpenDoor( this, r ) );
	closeActions().push_back( new CloseDoor( this, r ) );	
}

void	Door::makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs )
{
	for ( unsigned k = 0; k < openActions().size(); k++ )
	{
		openActions()[k]->makeSTRIPSAction( domain );
		QObject::connect( openActions()[k], SIGNAL( executed( unsigned ) ),
					obs, SLOT( actionExecuted( unsigned ) ) );
	}
	for ( unsigned k = 0; k < closeActions().size(); k++ )
	{
		closeActions()[k]->makeSTRIPSAction( domain );
		QObject::connect( closeActions()[k], SIGNAL( executed( unsigned ) ),
					obs, SLOT( actionExecuted( unsigned ) ) );
	}		
}

void	Door::makeSTRIPSFluents( STRIPS_Problem& domain )
{
	unsigned index = STRIPS_Problem::add_fluent( domain, QString( "(open %1)").arg( name() ).toStdString() );
	mOpenFluent = domain.fluents()[index];
	index = STRIPS_Problem::add_fluent( domain, QString( "(closed %1)").arg( name() ).toStdString() );
	mClosedFluent = domain.fluents()[index];
}

void	Door::evalSTRIPSFluents( aptk::Fluent_Vec& eval )
{
	if ( isOpen() ) 
		eval.push_back( openFluent()->index() );
	else
		eval.push_back( closedFluent()->index() );
}

void	Door::open()
{
	mOpen = true;
}

void	Door::close()
{
	mOpen = false;
}


}
