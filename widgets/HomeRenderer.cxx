#include <widgets/HomeRenderer.hxx>

namespace UI
{

HomeRenderer::HomeRenderer( QObject* parent ) 
	: QGraphicsScene( parent ), mFloorPlan( NULL )
{
}

HomeRenderer::~HomeRenderer()
{
}

void HomeRenderer::setFloorPlan( const QPixmap& pic )
{
	if ( mFloorPlan == NULL )
	{
		mFloorPlan = new QGraphicsPixmapItem( pic );
		mFloorPlan->setZValue( 0.0f );
		addItem( mFloorPlan  );
		return;
	}
	mFloorPlan->setPixmap(pic);
}

}
