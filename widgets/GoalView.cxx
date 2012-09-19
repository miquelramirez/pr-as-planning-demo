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
