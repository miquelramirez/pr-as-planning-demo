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

#ifndef __GOAL_VIEW__
#define __GOAL_VIEW__

#include <QFrame>
#include <QProgressBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>

namespace UI
{

class GoalView : public QFrame
{
	Q_OBJECT
public:
	GoalView( const QString& title, QWidget* parent = NULL );

	virtual ~GoalView();

	const QString&	goalName() { return mGoalName; }
	void		makeTooltip( const QStringList& list );
signals:
	
	void 	removeGoal( QString label );
	
public slots:

	void 	setProbability( float v );
	void	removalRequested();

protected:

	void 	setupWidgets();

protected:
	QString		mGoalName;
	QVBoxLayout*	mLayout;
	QLabel*		mGoalNameLabel;
	QLabel*		mProbDisplayLabel;
	QProgressBar*	mProbDisplay;
	QMenu*		mPopupMenu;
	QAction*	mRemoveAction;
	QWidget*	mProbView;
};

}

#endif // GoalView.hxx
