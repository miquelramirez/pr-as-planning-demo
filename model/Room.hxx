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

#ifndef __ROOM__
#define __ROOM__

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QDomElement>
#include <QMenu>
#include <QList>
#include <QMap>
#include <strips_prob.hxx>
#include <fluent.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;

namespace Planning
{
class Observer;
}

namespace Application
{

class Item;
class Door;
class WalkTo;
class Drop;
class PickUp;
class StageProp;

class Room : public QObject
{
	Q_OBJECT
public:

	Room( );
	virtual ~Room();

	static bool 			load( const QDomElement& elem, Room* obj );

	const QString& 			name() const { return mName; }
	const QPoint&			center() const { return mCenter; }
	const QVector< QPoint >&	geometry() const { return mGeometry; }

	QVector< Room* >&		neighbors()  { return mConnections; }
	const QVector< QString >&	neighborNames() const { return mConnPlaceholders; }
	QVector< Door* >&		doors() { return mDoors; }
	const QVector< QString >&	doorNames() const { return mDoorPlaceholders; }

	QVector< WalkTo* >&		walkToActions() { return mWalkToActions; }
	void				connectTo( Room* other, Door* );

	QList< Item*>&			contents() { return mContents; }
	QVector< PickUp* >&		pickUpActions() { return mPickUpActions; }
	QVector< Drop* >&		dropActions() { return mDropActions; }
	void				makeItemActions();

	QList< StageProp* >&		props() { return mStageProps; }

	void				setAgentLocation( bool v );
	bool				isAgentHere() const { return mAgentHere; }
	
	QMenu*				getActionMenu();	

	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs );
	void				evalSTRIPSFluents( aptk::Fluent_Vec& eval );

	Fluent*				atFluent() 		{ return mAtFluent; }
	QMap< QString, Fluent* >&	atItemFluents() 	{ return mAtItemFluents; }
	QMap< QString, Fluent* >&	atPropFluents()		{ return mAtPropFluents; } 

signals:
	
	void		showAgent();
	void		hideAgent();

public	slots:
	void		addItem( Item* i );
	void		removeItem( Item* i );

protected:

	QString				mName;
	QVector<QPoint>			mGeometry;
	QVector< QString >		mConnPlaceholders;
	QVector< QString >		mDoorPlaceholders;
	QVector< Room* >		mConnections;
	QVector< Door* >		mDoors;
	QPoint				mCenter;
	bool				mAgentHere;
	QMenu*				mActionMenu;
	QVector< WalkTo* >		mWalkToActions;
	QList< Item*>			mContents;
	QVector< PickUp* >		mPickUpActions;
	QVector< Drop* >		mDropActions;
	Fluent*				mAtFluent;
	QMap< QString, Fluent* >	mAtItemFluents;
	QList< StageProp* >		mStageProps;
	QMap< QString, Fluent* >	mAtPropFluents;
};

}

#endif // Room.hxx
