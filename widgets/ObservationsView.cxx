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
