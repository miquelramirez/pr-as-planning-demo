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

#ifndef __PLANNING_TASK__
#define __PLANNING_TASK__

#include <QObject>
#include <strips_prob.hxx>
#include <action.hxx>
#include <fstream>

using aptk::STRIPS_Problem;
using aptk::Action;
using aptk::Action_Ptr_Vec;


namespace Planning
{

class	PlanningTask : public QObject
{
	Q_OBJECT
public:

	PlanningTask( STRIPS_Problem& prob, Action_Ptr_Vec& obs, std::string logFileName, bool doReachabilityTest = false );
	virtual ~PlanningTask();

	float	result() const;
	
	bool			doReachabilityTest();
	static void		solve( PlanningTask* task );

protected:

	STRIPS_Problem&		mProblem;
	Action_Ptr_Vec&		mObsSequence;
	float			mCost;
	bool			mDoReachabilityTest;
	std::ofstream		mLogFile;
};

}

#endif // PlanningTask.hxx
