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

#include <model/Room.hxx>
#include <QApplication>
#include <QMessageBox>
#include <model/WalkTo.hxx>
#include <model/PickUp.hxx>
#include <model/Drop.hxx>
#include <iostream>
#include <planning/Observer.hxx>
#include <model/Door.hxx>
#include <model/OpenDoor.hxx>
#include <model/CloseDoor.hxx>
#include <model/Agent.hxx>
#include <model/Item.hxx>
#include <model/ItemManager.hxx>
#include <model/StageProp.hxx>
#include <model/StagePropAction.hxx>

namespace Application
{

Room::Room(  )
	: mAgentHere( false ), mActionMenu( NULL ), mAtFluent( NULL )
{
}

Room::~Room()
{
	if ( mActionMenu == NULL )
		delete mActionMenu;
}

void	Room::setAgentLocation( bool v ) 
{
	mAgentHere = v;
	if ( v ) 
		emit showAgent();
	else
		emit hideAgent();
}

void	Room::connectTo( Room* r, Door* d )
{
	neighbors().push_back( r );
	walkToActions().push_back( new WalkTo( this, r, d ) );
}

bool	Room::load( const QDomElement& root, Room* obj )
{
	if (!root.hasAttribute( "name" ) )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Document is not well formed "
			"'name' attribute missing in <Room> element."));
        	 return false;
	}

	obj->mName = root.attribute("name");	

	QDomElement geoElem = root.firstChildElement("Geometry");
	
	if ( geoElem.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Room '%1' has no <Geometry> defined").arg(obj->mName));
        	 return false;
	}

	QDomElement pointElem = geoElem.firstChildElement();
	while ( !pointElem.isNull() )
	{
		if ( pointElem.tagName() == "Point" )
		{
			QPoint point;
			point.setX( pointElem.attribute("x").toInt() );
			point.setY( pointElem.attribute("y").toInt() );
			obj->mGeometry.push_back( point );
		}
		pointElem = pointElem.nextSiblingElement();
	}

	if ( obj->mGeometry.isEmpty() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Room '%1' <Geometry> doesn't define any point!").arg(obj->mName));
        	 return false;		
	}

	QDomElement centerElem = root.firstChildElement( "Center" );
	if ( centerElem.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Room '%1' has no <Center> defined").arg(obj->mName));
        	 return false;
		
	}
	QDomElement centerPointElem = centerElem.firstChildElement( "Point" );
	if ( centerPointElem.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Room '%1' <Center> doesn't specify a Point").arg(obj->mName));
        	 return false;		
	}

	QDomElement connections = root.firstChildElement( "Connections" );
	if ( connections.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Room '%1' isn't connected to anything").arg(obj->mName));
        	 return false;		
	}
	QDomElement connElem = connections.firstChildElement();
	while ( !connElem.isNull() )
	{
		
		obj->mConnPlaceholders.push_back( connElem.attribute( "name" ) );
		if ( connElem.hasAttribute( "door" ) )
			obj->mDoorPlaceholders.push_back( connElem.attribute( "door" ) );
		else
			obj->mDoorPlaceholders.push_back( "null" );
			
		connElem = connElem.nextSiblingElement();
	}

	obj->mCenter.setX( centerPointElem.attribute("x").toInt() );
	obj->mCenter.setY( centerPointElem.attribute("y").toInt() );

	return true;
}

QMenu*	Room::getActionMenu()
{
	if ( mActionMenu != NULL )
		delete mActionMenu;
	mActionMenu = new QMenu(name());
	mActionMenu->addAction( name() );
	mActionMenu->addSeparator();
	QMenu*	walkToMenu = new QMenu( QString("Walk To") );
	for ( int k = 0; k < walkToActions().size(); k++ )
	{
		WalkTo* a = walkToActions()[k];
		if (!a->checkPrecondition()) continue;
		QAction* menuAction = walkToMenu->addAction( a->destName() );
		QObject::connect( menuAction, SIGNAL( triggered() ), a, SLOT( execute() ) );
	}
	mActionMenu->addMenu(walkToMenu);
	QMenu*	openDoorMenu = new QMenu( QString( "Open Door") );
	for ( int k = 0; k < doors().size(); k++ )
	{
		for ( int j = 0; j < doors()[k]->openActions().size(); j++ )
		{
			OpenDoor* a = doors()[k]->openActions()[j];
			if (!a->checkPrecondition()) continue;
			QAction* menuAction = openDoorMenu->addAction( a->doorName() );
			QObject::connect( menuAction, SIGNAL( triggered() ), a, SLOT( execute() ) );
		}
	}
	
	mActionMenu->addMenu(openDoorMenu);
	QMenu*	closeDoorMenu = new QMenu( QString( "Close Door") );
	for ( int k = 0; k < doors().size(); k++ )
	{
		for ( int j = 0; j < doors()[k]->closeActions().size(); j++ )
		{
			CloseDoor* a = doors()[k]->closeActions()[j];
			if (!a->checkPrecondition()) continue;
			QAction* menuAction = closeDoorMenu->addAction( a->doorName() );
			QObject::connect( menuAction, SIGNAL( triggered() ), a, SLOT( execute() ) );
		}
	}
	QMenu* pickUpMenu = new QMenu( QString( "Pick Up" ) );
	for ( int i = 0; i < pickUpActions().size(); i++ )
	{
		PickUp*	a = pickUpActions()[i];
		if (!a->checkPrecondition()) continue;
		QAction* menuAction = pickUpMenu->addAction( a->itemName() );
		QObject::connect( menuAction, SIGNAL(triggered()), a, SLOT(execute()));
	} 
	mActionMenu->addMenu(pickUpMenu);
	QMenu* dropMenu = new QMenu( QString( "Drop" ) );
	for ( int i = 0; i < pickUpActions().size(); i++ )
	{
		Drop*	a = dropActions()[i];
		if (!a->checkPrecondition()) continue;
		QAction* menuAction = dropMenu->addAction( a->itemName() );
		QObject::connect( menuAction, SIGNAL(triggered()), a, SLOT(execute()));
	} 
	mActionMenu->addMenu(dropMenu);
	mActionMenu->addMenu(closeDoorMenu);

	QMenu* propsMenu = new QMenu( QString( "Props Actions" ) );
	for ( int i = 0; i < props().size(); i++ )
	{
		QMenu* propActionMenu = new QMenu( props()[i]->name() );
		StageProp* prop = props()[i];
		for (  QMap< QString, StagePropAction* >::Iterator propActionIt = prop->actions().begin();
			propActionIt != prop->actions().end(); propActionIt++ )
		{
			if ( !propActionIt.value()->checkPrecondition() ) continue;
			QAction* menuAction = propActionMenu->addAction( propActionIt.key() );
			QObject::connect( menuAction, SIGNAL(triggered()), propActionIt.value(), SLOT(execute()) );
		}
		propsMenu->addMenu( propActionMenu );	
	}
	mActionMenu->addMenu( propsMenu );

	return mActionMenu;
}

void	Room::makeSTRIPSFluents( STRIPS_Problem& domain )
{
	unsigned index = STRIPS_Problem::add_fluent( domain, QString( "(at %1)").arg( name() ).toStdString() );
	mAtFluent = domain.fluents()[index];
	QList< Item* >&	itemList = ItemManager::instance().items();
	for ( int i = 0; i < itemList.size(); i++ )
	{
		index = STRIPS_Problem::add_fluent( domain, QString( "(at %1 %2)" ).arg(name()).arg( itemList[i]->name() ).toStdString() );
		mAtItemFluents[ itemList[i]->name() ] = domain.fluents()[index];
	}
	for ( int i = 0; i < mStageProps.size(); i++ )
	{
		index = STRIPS_Problem::add_fluent( domain, QString( "(at %1 %2)" ).arg(name()).arg( mStageProps[i]->name() ).toStdString() );
		mAtPropFluents[ mStageProps[i]->name() ] = domain.fluents()[index];
	}

}

void	Room::makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs )
{
	for ( int i = 0; i < walkToActions().size(); i++ )
	{
		walkToActions()[i]->makeSTRIPSAction( domain );
		QObject::connect( walkToActions()[i], SIGNAL( executed(unsigned) ), obs, SLOT( actionExecuted(unsigned) ));
	}
	makeItemActions();
	for ( int i = 0; i < pickUpActions().size(); i++ )
	{
		pickUpActions()[i]->makeSTRIPSAction( domain );
		QObject::connect( pickUpActions()[i], SIGNAL(executed(unsigned)), obs, SLOT(actionExecuted(unsigned)));
	}
	for ( int i = 0; i < dropActions().size(); i++ )
	{
		dropActions()[i]->makeSTRIPSAction( domain );
		QObject::connect( dropActions()[i], SIGNAL(executed(unsigned)), obs, SLOT(actionExecuted(unsigned)));
	}

}

void	Room::evalSTRIPSFluents( aptk::Fluent_Vec& eval )
{
	// At fluent
	if ( isAgentHere() ) eval.push_back( atFluent()->index() );
	for ( int k = 0; k < contents().size(); k++ )
		eval.push_back( atItemFluents()[ contents()[k]->name() ]->index() );
	for ( int i = 0; i < mStageProps.size(); i++ ) 
	{
		eval.push_back( mAtPropFluents[ mStageProps[i]->name() ]->index() );
		mStageProps[i]->evalSTRIPSFluents( eval );
	}
}

void	Room::addItem( Item* i )
{
	contents().push_back( i );
}

void	Room::removeItem( Item* i )
{
	contents().removeOne( i );
}

void	Room::makeItemActions( )
{
	Agent&	agent = Agent::instance();
	QList< Item* >& itemList = ItemManager::instance().items();

	for ( int i = 0; i < itemList.size(); i++ )
	{
		pickUpActions().push_back( new PickUp( &agent, this, itemList[i] ) );
		dropActions().push_back( new Drop( &agent, this, itemList[i] ) );
	}
}

}
