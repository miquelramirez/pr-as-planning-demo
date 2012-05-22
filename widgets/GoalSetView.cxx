#include <widgets/GoalSetView.hxx>
#include <widgets/NewGoalDialog.hxx>
#include <iostream>
#include <QMouseEvent>

namespace UI
{

GoalSetView::GoalSetView( QWidget* parent )
	: QGroupBox( parent ), mFrozen( true )
{
	setup();
}

GoalSetView::GoalSetView( const QString& title, QWidget* parent )
	: QGroupBox( title, parent ), mFrozen( true )
{
	setup();
}

GoalSetView::~GoalSetView()
{
	delete mViewLayout;
}

void	GoalSetView::freeze()
{
	mFrozen = true;
}

void	GoalSetView::unFreeze()
{
	mFrozen = false;
}

void	GoalSetView::setup()
{
	mViewLayout = new QVBoxLayout( this ); 
	mViewLayout->setObjectName( QString::fromUtf8("ViewLayout") );
	mViewLayout->setContentsMargins( 10, 10, 10, 10 );

	mGoalsArea = new QScrollArea( this );
	mGoalsAreaLayout = new QVBoxLayout( mGoalsArea );
	mGoalsAreaLayout->setContentsMargins( 5, 5, 5, 5 );

	mViewLayout->addWidget( mGoalsArea );

	mMenu = new QMenu;
	mCreateNewGoal = mMenu->addAction("Add New Goal...");
	mClearGoals = mMenu->addAction( "Clear Goals" );
	
	QObject::connect( mCreateNewGoal, SIGNAL( triggered() ), this, SLOT( createNewGoalRequested() ) );
	QObject::connect( mClearGoals, SIGNAL( triggered() ), this, SLOT( clearGoals() ) );	
}

void	GoalSetView::createNewGoalRequested()
{
	emit requestHypGoalsInfo();
}

void	GoalSetView::acceptHypGoalsInfo( QList< QString >& fluents, QList< QString >& goals )
{
	std::cout << "Create new goal" << std::endl;
	NewGoalDialog* dlg = new NewGoalDialog( "Create New Goal", fluents, goals );
	if ( dlg->exec() == QDialog::Accepted )
	{
		emit createNewGoal( dlg->goalName(), dlg->fluentsInGoal() );
	}
	delete dlg;
}

void	GoalSetView::clearGoals()
{
	std::cout << "Clearing all goals" << std::endl;
}

void	GoalSetView::mousePressEvent( QMouseEvent* event )
{
		
	if ( mFrozen )
		std::cout << "Frozen" << std::endl;

	if ( !mFrozen && event->button() == Qt::RightButton )
		mMenu->popup( event->globalPos() );
		
}

void	GoalSetView::removeGoalView( QString name )
{
	QList< GoalView* >::iterator it;
	for ( it = mGoalViews.begin();
		it != mGoalViews.end(); it++ )
	{
		if ( (*it)->goalName() == name ) 
			break;		
	}

	if ( it == mGoalViews.end() ) return;
	
	GoalView* v = *it;
	mGoalViews.erase( it );

	v->hide();
	v->setParent( NULL );
	mGoalsAreaLayout->removeWidget( v );
	
	delete v;
}

void	GoalSetView::addNewGoalView( GoalView* v )
{
	v->setParent( this );	
	mGoalsAreaLayout->addWidget( v );
	mGoalViews.push_back( v );
	QObject::connect( v, SIGNAL( removeGoal( QString ) ), this, SLOT( removeGoalView( QString ) ) );
}

} 
