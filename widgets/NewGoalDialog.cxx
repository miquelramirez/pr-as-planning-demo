#include <widgets/NewGoalDialog.hxx>
#include <iostream>
#include <cassert>
#include <QMessageBox>

namespace UI
{

NewGoalDialog::NewGoalDialog( const QString& title, QList< QString >& fluents, QList< QString >& goals, QWidget* parent )
	: QDialog( parent ), mExistingGoals( goals )
{
	mFluentsModel.setStringList( fluents );
	setupWidgets();
	setWindowTitle( title );
}

NewGoalDialog::~NewGoalDialog()
{
}

void	NewGoalDialog::setupWidgets()
{
	mDialogLayout = new QVBoxLayout( this );
	mDialogLayout->setContentsMargins( 20,20,20,20 );	

	setupGoalNameGroup();
	setupFluentsGroup();
	setupFluentsControlGroup();	

	mDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);

	connect(mDialogButtonBox, SIGNAL(accepted()), this, SLOT(verify()));
	connect(mDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mDialogLayout->addWidget( mGoalNameGroup );
	mDialogLayout->addWidget( mFluentsWidget );
	mDialogLayout->addWidget( mFluentsControlWidget );
	mDialogLayout->addWidget( mDialogButtonBox );
}

void	NewGoalDialog::setupGoalNameGroup()
{
	mGoalNameGroup = new QWidget( this );
	mGoalNameLayout = new QVBoxLayout( mGoalNameGroup );
	mGoalNameLabel = new QLabel( "Goal Name", mGoalNameGroup);
	mGoalNameInput = new QLineEdit( mGoalNameGroup );
	mGoalNameLayout->addWidget( mGoalNameLabel );
	mGoalNameLayout->addWidget( mGoalNameInput );
	mGoalNameGroup->setMinimumSize( QSize( 600, 80 ) );
}

void	NewGoalDialog::setupFluentsGroup()
{
	mFluentsWidget = new QWidget( this );
	mFluentsWidget->setMinimumSize( QSize( 600, 320 ) );
	
	mFluentsWidgetLayout = new QHBoxLayout( mFluentsWidget );
	mFluentsWidgetLayout->setContentsMargins( 5, 5, 5, 5 );
	
	mAvailableFluentsGroup = new QWidget( mFluentsWidget );
	mAvailableFluentsLayout = new QVBoxLayout ( mAvailableFluentsGroup );
	mAvailableFluentsLabel = new QLabel( "Available Fluents", mAvailableFluentsGroup );
	mAvailableFluents = new QListView( mAvailableFluentsGroup );
	mAvailableFluents->setModel( &mFluentsModel );
		
	mAvailableFluentsLayout->addWidget( mAvailableFluentsLabel );
	mAvailableFluentsLayout->addWidget( mAvailableFluents );
	
	mFluentsInGoalGroup = new QWidget( mFluentsWidget );
	mFluentsInGoalLayout = new QVBoxLayout( mFluentsInGoalGroup );
	mFluentsInGoalLabel = new QLabel( "Fluents in Goal", mFluentsInGoalGroup );
	mFluentsInGoal = new QListView( mFluentsInGoalGroup );
	mFluentsInGoal->setModel( &mFluentsInGoalModel );
	mFluentsInGoalLayout->addWidget( mFluentsInGoalLabel );
	mFluentsInGoalLayout->addWidget( mFluentsInGoal );

	mFluentsWidgetLayout->addWidget( mAvailableFluentsGroup );
	mFluentsWidgetLayout->addWidget( mFluentsInGoalGroup );
}

void	NewGoalDialog::setupFluentsControlGroup()
{
	mFluentsControlWidget = new QWidget( this );
	mFluentsControlWidget->setMinimumSize( QSize( 600, 80 ) );
	
	mFluentsControlLayout = new QHBoxLayout( mFluentsControlWidget );
	mFluentsControlLayout->setContentsMargins( 5,5,5,5 );

	mAddFluent = new QPushButton( mFluentsControlWidget );
	mAddFluent->setText( "&Add" );
	QObject::connect( mAddFluent, SIGNAL( clicked() ), this, SLOT( onAddFluent() ) );
	mRemoveFluent = new QPushButton( mFluentsControlWidget );
	mRemoveFluent->setText( "&Remove" );
	QObject::connect( mRemoveFluent, SIGNAL( clicked() ), this, SLOT( onRemoveFluent() ) );
	mClearFluents = new QPushButton( mFluentsControlWidget );
	mClearFluents->setText( "C&lear" );
	QObject::connect( mClearFluents, SIGNAL( clicked() ), this, SLOT( onClearFluent() ) );

	mFluentsControlLayout->addWidget( mAddFluent );
	mFluentsControlLayout->addWidget( mRemoveFluent );
	mFluentsControlLayout->addWidget( mClearFluents );
}

void	NewGoalDialog::verify()
{
	if ( mGoalNameInput->text().isEmpty() )
	{
		QMessageBox::warning( this, tr("Sim Home"), tr("Goals must be named") );
		return;
	}

	for ( int i = 0; i < mFluentsInGoalModel.rowCount(); i++ )
	{
		QModelIndex idx = mFluentsInGoalModel.index( i );
		QVariant datum = mFluentsInGoalModel.data( idx, Qt::DisplayRole );
		mFluentsInGoalList.push_back( datum.toString() );
	}	

	if ( mFluentsInGoalList.isEmpty() )
	{
		QMessageBox::warning( this, tr("Sim Home"), tr("Goals can't be empty") );
		return;		
	}

	accept();
}

void	NewGoalDialog::onAddFluent()
{
	QModelIndex currIdx = mAvailableFluents->currentIndex();
	QVariant datum = mFluentsModel.data( currIdx, Qt::DisplayRole );
	QString	selected = datum.toString();
	std::cout << "Selected:" << selected.toStdString() << std::endl;

	mFluentsInGoalModel.insertRows( mFluentsInGoalModel.rowCount(), 1 );
	mFluentsInGoalModel.setData( mFluentsInGoalModel.index( mFluentsInGoalModel.rowCount()-1 ), datum, Qt::DisplayRole );

	mFluentsInGoal->reset();	
}

void	NewGoalDialog::onRemoveFluent()
{
	std::cout << "Removing fluent" << std::endl;
	QModelIndex currIdx = mFluentsInGoal->currentIndex();
	mFluentsInGoalModel.removeRow( currIdx.row() );	
	mFluentsInGoal->reset();	
}

void	NewGoalDialog::onClearFluent()
{
	while ( mFluentsInGoalModel.rowCount() > 0 )
		mFluentsInGoalModel.removeRow( 0 );
	mFluentsInGoal->reset();	
}

}
