#ifndef __HOME_RENDERER__
#define __HOME_RENDERER__

#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>

namespace UI
{

class HomeRenderer : public QGraphicsScene
{
	Q_OBJECT

public:

	HomeRenderer( QObject* parent = NULL );
	virtual ~HomeRenderer();

public slots:

	void	setFloorPlan( const QPixmap& pic );

private:

	QGraphicsPixmapItem*	mFloorPlan;

};

}

#endif // HomeRenderer.hxx
