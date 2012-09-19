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

#ifndef __OBSERVER__
#define __OBSERVER__

#include <QObject>
#include <strips_prob.hxx>
#include <vector>
#include <list>
#include <utility>
#include <planning/Goal.hxx>
#include <widgets/GoalView.hxx>

using aptk::STRIPS_Problem;
using aptk::Action_Ptr_Vec;

namespace Planning
{

class Observer : public QObject
{
	Q_OBJECT
public:

	Observer( STRIPS_Problem& domain );
	virtual ~Observer();

	bool	started() const { return mStarted; }
	float	obsLevel() const { return mObsLevel; }

signals:
	void	requestInitialStateUpdate();
	void	goalViewCreated( UI::GoalView* v );
	void	sendHypGoalsInfo( QList< QString >&, QList< QString >& );
	void	actionObserved( QString a );

public slots:
	void	start();
	void	stop();
	void	setObsLevel( float level );
	void	actionExecuted( unsigned index );
	void	updateInitialState( aptk::Fluent_Vec& eval );
	void	addNewGoal( QString label, QList< QString >& fluents );
	void	removeGoal( QString label );
	void	onHypGoalsInfoRequested();

protected:

	STRIPS_Problem&		mDomain;
	Action_Ptr_Vec		mObservations;
	bool			mStarted;
	float			mObsLevel;
	aptk::Fluent_Vec	mInitialState;
	std::list< Goal* >	mHypGoals;
	
};

}

#endif
