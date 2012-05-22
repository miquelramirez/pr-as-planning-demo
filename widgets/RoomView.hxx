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
