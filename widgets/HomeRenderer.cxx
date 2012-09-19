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
