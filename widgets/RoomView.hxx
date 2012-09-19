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

#ifndef __ROOM_VIEW__
#define __ROOM_VIEW__

#include <QGraphicsPolygonItem>
#include <QPolygon>
#include <QObject>

namespace Application
{

class Room;

}

namespace UI
{

class RoomView :  public QObject, public QGraphicsPolygonItem
{
	Q_OBJECT
public:

	RoomView( Application::Room* obj, QGraphicsItem* parent = NULL );
	virtual ~RoomView();

	Application::Room&		observed() { return *mObservedRoom; }
	const Application::Room&	observed() const { return *mObservedRoom; }

public slots:

	void	showAgent();
	void 	hideAgent();

protected:
	
	virtual void		hoverEnterEvent(QGraphicsSceneHoverEvent* evt);
	virtual void		hoverLeaveEvent(QGraphicsSceneHoverEvent* evt);
	virtual	void		mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void		contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected:

	void	setupView();

protected:

	Application::Room* 		mObservedRoom;
	QGraphicsPixmapItem*		mAgentAvatar;
};


};

#endif // RoomView.hxx
