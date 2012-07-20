#include <widgets/GoalView.hxx>
#include <QLabel>

namespace UI
{

GoalView::GoalView( const QString& title, QWidget* parent )
	: QFrame( parent ), mGoalName( title )
{
	setupWidgets();
}

GoalView::~GoalView()
{
}

void 	GoalView::makeTooltip( const QStringList& list )
{
	QString tooltip;
	for ( int i = 0; i < list.size(); i++ )
	{
		tooltip.append( list[i] );
		if ( i < list.size()-1 )
			tooltip.append( "\n" );
	}
	setToolTip( tooltip );
}

void	GoalView::setupWidgets()
{
	setMinimumWidth( 150 );
	setMaximumHeight( 80 );
	setMinimumHeight( 80 );
	setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

	mLayout = new QVBoxLayout( this );
	mLayout->setObjectName( QString::fromUtf8( "GoalLayout" ) );
	mLayout->setContentsMargins( 5, 5, 5, 5 );

	mGoalNameLabel = new QLabel( mGoalName, this );
	mLayout->addWidget( mGoalNameLabel );

	mProbView = new QWidget( this );

	QHBoxLayout* probViewLayout = new QHBoxLayout( mProbView );
	probViewLayout->setObjectName( QString::fromUtf8( "GoalProbLayout" ) );
	probViewLayout->setContentsMargins( 1, 1, 1, 1 );

	mProbDisplayLabel = new QLabel( "P(G|O)", mProbView );
	mLayout->addWidget( mProbDisplayLabel );	

	mProbDisplay = new QProgressBar( mProbView );
	mProbDisplay->setMinimum( 0 );
	mProbDisplay->setMaximum( 100 );
	mProbDisplay->setValue( 0 );	

	probViewLayout->addWidget( mProbDisplayLabel );
	probViewLayout->addWidget( mProbDisplay );

	mLayout->addWidget( mProbView );

	mPopupMenu = new QMenu;
	mRemoveAction = new QAction( this );
	mRemoveAction->setText( "Remove" );
	QObject::connect( mRemoveAction, SIGNAL( triggered() ), this, SLOT( removalRequested() ) );
	mPopupMenu->addAction( mRemoveAction );	
}

void	GoalView::setProbability( float v )
{
	mProbDisplay->setValue( (float)mProbDisplay->maximum()*v );
}

void	GoalView::removalRequested( )
{
	emit removeGoal( goalName() );
}

}
