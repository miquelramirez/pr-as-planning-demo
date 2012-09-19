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
 
#include <widgets/ObservationsView.hxx>
#include <QVBoxLayout>
#include <iostream>

namespace UI
{

ObservationsView::ObservationsView( QWidget* parent )
	: QGroupBox( parent )
{
	setup();
}

ObservationsView::ObservationsView( const QString& title, QWidget* parent )
	: QGroupBox( title, parent )
{
	setup();
}
	
ObservationsView::~ObservationsView()
{
	delete mActionLogView;
}

void	ObservationsView::setup()
{
	QVBoxLayout* layout = new QVBoxLayout( this );
	layout->setObjectName( QString::fromUtf8( "ObservationsViewLayout" ) );
	layout->setContentsMargins( 10, 10, 10, 10 );

	mActionLogView = new QListView( this );
	mActionLogView->setModel( &mActionLog );
	mActionLogView->setMinimumHeight( 300 );
	layout->addWidget( mActionLogView );
}

void	ObservationsView::logAction( QString signature )
{
	std::cout << "Observed action " << signature.toStdString() << " logged" << std::endl;
	mActionLog.insertRows( mActionLog.rowCount(), 1 );
	
	mActionLog.setData( 	mActionLog.index( mActionLog.rowCount() - 1 ),
				QVariant(signature) , Qt::DisplayRole );
	mActionLogView->reset();
}

void	ObservationsView::clearActionLog()
{
	while (mActionLog.rowCount() > 0)
		mActionLog.removeRow(0);
	mActionLogView->reset();
}

}
