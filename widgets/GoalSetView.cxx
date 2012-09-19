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
	delete mGoalListLayout;
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
	mViewLayout->setObjectName( QString::fromUtf8( "GoalSetViewLayout" ) );
	mViewLayout->setContentsMargins( 5, 5, 5, 5 );

	mGoalsList = new QFrame( this );
	mGoalsList->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
	mGoalsList->setMinimumHeight( 400 );
	
	mGoalsScrollingManager = new QScrollArea;
	mGoalsScrollingManager->setWidget( mGoalsList );

	mGoalListLayout = new QVBoxLayout( mGoalsList );
	mGoalListLayout->setContentsMargins( 25, 25, 25, 25 );


	mViewLayout->addWidget( mGoalsList );

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
	mGoalListLayout->removeWidget( v );
	
	delete v;
}

void	GoalSetView::addNewGoalView( GoalView* v )
{
	v->setParent( mGoalsList );	
	mGoalListLayout->addWidget( v );
	mGoalViews.push_back( v );
	QObject::connect( v, SIGNAL( removeGoal( QString ) ), this, SLOT( removeGoalView( QString ) ) );
}

} 
