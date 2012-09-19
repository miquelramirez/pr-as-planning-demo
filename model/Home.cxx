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

#include <model/Home.hxx>
#include <model/Room.hxx>
#include <model/Door.hxx>
#include <model/ItemManager.hxx>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <iostream>
#include <QMap>
#include <cstdlib>
#include <StageProp.hxx>

namespace Application
{

Home::Home( )
{
}

Home::~Home()
{
	for ( unsigned k = 0; k < mRooms.size(); k++ )
		delete mRooms[k];
	for ( unsigned k = 0; k < mDoors.size(); k++ )
		delete mDoors[k];
	for ( unsigned k = 0; k < mProps.size(); k++ )
		delete mProps[k];
}

void	Home::loadFloorPlan( QDomElement& elem )
{
	mFloorPlanImg.load(elem.attribute("path"));
}

bool	Home::connectRooms()
{
	QMap< QString, Room* > roomsTable;
	
	for ( unsigned k = 0; k < rooms().size(); k++ )
		roomsTable.insert( rooms()[k]->name(), rooms()[k] );
	
	for ( unsigned k = 0; k < rooms().size(); k++ )
	{
		const QVector< QString >& names = rooms()[k]->neighborNames();
		const QVector< QString >& doorNames = rooms()[k]->doorNames();
		assert( names.size() == doorNames.size() );
		for ( int l = 0; l < names.size(); l++ )
		{
			if ( doorNames[l] != "null" )
			{
				mDoorsTable[doorNames[l]]->addRoom( rooms()[k] );
				rooms()[k]->doors().push_back( mDoorsTable[doorNames[l]] );
			}	
			Room* neighbor = roomsTable[ names[l] ];
			if ( neighbor == NULL )
			{
				QMessageBox::information( QApplication::activeWindow(), 
						tr("Sim Home"),
						tr("Could not find Room '%1' neighbor '%2'!")
						.arg(rooms()[k]->name())
						.arg(names[l]));
				return false;
			}
			rooms()[k]->connectTo( neighbor, (doorNames[l] == "null" ? NULL : mDoorsTable[doorNames[l]] ) );
		}
	}
	
	return true;		
}

bool	Home::load( const QString& xmlDocument, Home* obj )
{
	QFile	xmlDocFile( xmlDocument );
	QString	errorStr;
	int errorLine, errorColumn;
	QDomDocument 	xmlDoc;	

	if (!xmlDoc.setContent( &xmlDocFile, true, &errorStr, &errorLine, &errorColumn ) )
	{
		QMessageBox::information(QApplication::activeWindow(), tr("Sim Home"),
                                  tr("Error while parsing home document: %1 \nat line %2, column %3:\n%4")
				  .arg(xmlDocument)
                                  .arg(errorLine)
                                  .arg(errorColumn)
                                  .arg(errorStr));
		return false;
	}

	QDomElement root = xmlDoc.documentElement();
	std::cout << root.tagName().toStdString() << std::endl;
	if (root.tagName() != "Home") 
	{
	       	QMessageBox::information(QApplication::activeWindow(), tr("Sim Home"),
		     			tr("The file is not a Home description"));
		return false;
	} 
	else if ( !root.hasAttribute("name") )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Home specification document is not well formed! "
			"'name' attribute missing in root element."));
        	 return false;
   	}
	QDomElement fpElem = root.firstChildElement("Floorplan");
	
	if ( fpElem.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("No floor plan image found for the Home in %1").arg(xmlDocument));
        	 return false;
	}
	obj->loadFloorPlan( fpElem );

	if ( !ItemManager::instance().loadItems( root ) )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Could not load Items") );
        	 return false;
	}

	QDomElement propsElem = root.firstChildElement( "StageProps" );
	
	if ( !propsElem.isNull() )
	{
		QDomElement child = propsElem.firstChildElement();
		while ( !child.isNull() )
		{
			if ( child.tagName() == "Prop" )
			{
				StageProp* p = new StageProp();
				if ( !StageProp::load( child, p ) )
					return false;
				std::cout << "Stage prop found: " << p->name().toStdString() << std::endl;
				obj->mProps.push_back( p );
				obj->mPropsTable[ p->name() ] = p;
			}
			
			child = child.nextSiblingElement();
			
		}
	}

	QDomElement roomsElem = root.firstChildElement( "Rooms" );

	if ( roomsElem.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Home in %1 has no rooms!").arg(xmlDocument));
        	 return false;
	}

	QDomElement child = roomsElem.firstChildElement();
	while ( !child.isNull() )
	{
		if ( child.tagName() == "Room" )
		{
			Room* r = new Room();
			if ( !Room::load( child, r ) )
				return false;
			std::cout << "Room found: " << r->name().toStdString() << std::endl;
			obj->mRooms.push_back( r );
			obj->mRoomsTable[ r->name() ] = r;
		}		

		child = child.nextSiblingElement();
	}

	QDomElement doorsElem = root.firstChildElement( "Doors" );
	if ( !doorsElem.isNull() )
	{
		QDomElement child = doorsElem.firstChildElement();
		while (	!child.isNull() )
		{
			Door* d = new Door();
			if (!Door::load( child, d ))
				return false;
			std::cout << "Door found: " << d->name().toStdString() << std::endl;
			obj->mDoors.push_back( d );
			obj->mDoorsTable[ d->name() ] = d;
			child = child.nextSiblingElement();
		}
	}

	QDomElement initElem = root.firstChildElement( "InitialAgentLocation" );
	if ( !initElem.isNull() ) 
	{
		for ( unsigned k = 0; k < obj->rooms().size(); k++ )
		{
			if ( obj->rooms()[k]->name() == initElem.attribute("name") )
				obj->rooms()[k]->setAgentLocation(true);
		}
	}

	obj->placeItems();
	obj->placeProps();

	return obj->connectRooms();
}

void	Home::placeItems()
{
	QList< Item* >&	itemList = ItemManager::instance().items();
	for ( int i = 0; i < itemList.size(); i++ )
	{
		if ( mRoomsTable.find( itemList[i]->initialLocation() ) == mRoomsTable.end() )
		{
			QMessageBox::information( QApplication::activeWindow(), 
					tr("Sim Home"),
					tr("Item %1 refers to Room %2, which hasn't been found")
					.arg( itemList[i]->name() ).arg( itemList[i]->initialLocation()) );
			std::exit(1);
		}
		mRoomsTable[ itemList[i]->initialLocation() ]->addItem( itemList[i] );
	}

}

void	Home::placeProps()
{
	for ( int i = 0; i < mProps.size(); i++ )
	{
		if ( mRoomsTable.find( mProps[i]->locationName() ) == mRoomsTable.end() )
		{
			QMessageBox::information( QApplication::activeWindow(), 
					tr("Sim Home"),
					tr("Stage Prop %1 refers to Room %2, which hasn't been found")
					.arg( mProps[i]->name() ).arg( mProps[i]->locationName()) );
			std::exit(1);
			
		}
		mRoomsTable[ mProps[i]->locationName() ]->props().push_back( mProps[i] );
		mProps[i]->setRoom( mRoomsTable[ mProps[i]->locationName() ] );
	}
}

}
