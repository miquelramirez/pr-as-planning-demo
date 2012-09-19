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

#ifndef __GOALSETVIEW__
#define __GOALSETVIEW__

#include <QGroupBox>
#include <QVBoxLayout>
#include <QMenu>
#include <QList>
#include <QFrame>
#include <QScrollArea>
#include <widgets/GoalView.hxx>
#include <widgets/ObservationsView.hxx>

namespace UI
{

class GoalSetView : public QGroupBox
{
	Q_OBJECT
public:
	GoalSetView( QWidget* parent = NULL );
	GoalSetView( const QString& title, QWidget* parent = NULL );
	virtual ~GoalSetView();

signals:
	
	void	requestHypGoalsInfo();
	void	createNewGoal( QString name, QList< QString >& fluents );

public slots:

	void	createNewGoalRequested();
	void	acceptHypGoalsInfo( QList< QString >& fluents, QList< QString >& goals );

	void	addNewGoalView( UI::GoalView* v );
	void	removeGoalView( QString name );
	void	clearGoals();
	void	freeze();
	void	unFreeze();

protected:

	void		setup();
	virtual void	mousePressEvent ( QMouseEvent * event );
protected:

	QMenu*			mMenu;
	QAction*		mCreateNewGoal;
	QAction*		mClearGoals;
	bool			mFrozen;
	QVBoxLayout*		mViewLayout;
	QFrame*			mGoalsList;
	QVBoxLayout*		mGoalListLayout;
	QScrollArea*		mGoalsScrollingManager;
	QList< GoalView* >	mGoalViews;
};

}

#endif // GoalSetView.hxx
