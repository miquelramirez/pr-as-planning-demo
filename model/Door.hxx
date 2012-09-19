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

#ifndef __DOOR__
#define __DOOR__

#include <QObject>
#include <QDomElement>
#include <QVector>
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

class	Room;
class	OpenDoor;
class	CloseDoor;

class	Door	: public QObject
{
	Q_OBJECT
public:
	Door();
	virtual ~Door();

	static bool 			load( const QDomElement& elem, Door* obj );

	const QString &			name() const { return mName; }
	void				addRoom( Room* r );

	QVector< OpenDoor* >&		openActions() { return mOpenActions; }
	QVector< CloseDoor* >&		closeActions() { return mCloseActions; }
	
	bool				isOpen() { return mOpen; }
	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs );
	void				evalSTRIPSFluents( aptk::Fluent_Vec& eval );
	Fluent*				openFluent() { return mOpenFluent; }
	Fluent*				closedFluent() { return mClosedFluent; }

public slots:

	void				open();
	void				close();

protected:
	QString				mName;
	std::vector< Room* >		mRooms;
	QVector< OpenDoor* >		mOpenActions;
	QVector< CloseDoor* >		mCloseActions;
	bool				mOpen;
	Fluent*				mOpenFluent;
	Fluent*				mClosedFluent;
		

};


}

#endif // Door.hxx
