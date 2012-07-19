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
