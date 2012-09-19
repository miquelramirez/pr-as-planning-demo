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

#ifndef __HOME__
#define __HOME__

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QDomElement>
#include <vector>
#include <QMap>

namespace Application
{

class Room;
class Door;
class StageProp;

class	Home	: public QObject
{
	Q_OBJECT
public:
	
	Home(  );
	virtual	~Home();

	const QString&			name() const 		{ return mName; }
	const QPixmap&			floorPlan() const  	{ return mFloorPlanImg; }
	std::vector<Room* >&		rooms() 		{ return mRooms; }
	std::vector<Door* >&		doors()			{ return mDoors; }
	std::vector<StageProp* >&	props()			{ return mProps; }
	Door*				getDoorByName( QString name ) { return mDoorsTable[name]; } 	

	static bool			load( const QString& xmlDocument, Home* obj );

protected:
	
	bool				connectRooms();
	void				placeItems();
	void				placeProps();
	void				loadFloorPlan( QDomElement& elem );

protected:
	QString				mName;
	std::vector< Room* >		mRooms;
	QMap< QString, Room* >		mRoomsTable;
	QPixmap				mFloorPlanImg;
	std::vector< Door* >		mDoors;
	QMap< QString, Door* >		mDoorsTable;
	std::vector< StageProp* >	mProps;
	QMap< QString, StageProp* >	mPropsTable;
 
};

}

#endif // Home.hxx
