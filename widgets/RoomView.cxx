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

#include <widgets/RoomView.hxx>
#include <widgets/GraphicsFactory.hxx>
#include <model/Room.hxx>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <iostream>
#include <QGraphicsSceneContextMenuEvent>

namespace UI
{

RoomView::RoomView( Application::Room* obj, QGraphicsItem* parent )
	: QGraphicsPolygonItem( parent )
{
	mObservedRoom = obj;
	setupView();
}

RoomView::~RoomView()
{
}

void	RoomView::hoverEnterEvent(QGraphicsSceneHoverEvent* )
{
	QBrush brush(  QColor( 31, 200, 55, 127 ), Qt::SolidPattern );
	setBrush( brush );
	QPen pen( QColor( 31, 200, 55, 255 ) );
	pen.setStyle( Qt::SolidLine );
	pen.setWidth( 3 );
	pen.setCapStyle( Qt::RoundCap );
	pen.setJoinStyle( Qt::RoundJoin );
	setPen( pen );
}

void	RoomView::hoverLeaveEvent( QGraphicsSceneHoverEvent* )
{
	QBrush brush( QColor( 255, 255, 255, 0 ), Qt::NoBrush );
	setBrush( brush );
	setPen( QPen( QColor( 255, 255, 255, 0 ) ) );
}

void	RoomView::mousePressEvent( QGraphicsSceneMouseEvent* )
{
	std::cout << "Right button on: " << observed().name().toStdString() << std::endl;	
}

void	RoomView::contextMenuEvent( QGraphicsSceneContextMenuEvent* evt )
{
	std::cout << "Context menu event on: " << observed().name().toStdString() << std::endl;
	QMenu* contextMenu = observed().getActionMenu();
	contextMenu->popup(evt->screenPos());

}

void	RoomView::setupView()
{
	QPolygon poly;
	for ( int k = 0; k < observed().geometry().size(); k++ )
		poly << observed().geometry()[k];
	setPolygon( poly );
	setZValue( 15.0f );
	setAcceptHoverEvents( true );
	setAcceptedMouseButtons( Qt::RightButton );
	hoverLeaveEvent(NULL);

	const QPixmap& picture = *(GraphicsFactory::instance().get_pixmap("agent"));
	mAgentAvatar = new QGraphicsPixmapItem( picture , this);
	mAgentAvatar->setOffset( observed().center()-QPoint(picture.width()/2, picture.height()/2) );
	mAgentAvatar->setZValue( 10.0f );
	if ( !observed().isAgentHere() )
		mAgentAvatar->hide();	
}

void	RoomView::showAgent()
{
	mAgentAvatar->show();
}

void	RoomView::hideAgent()
{
	mAgentAvatar->hide();
}

}
