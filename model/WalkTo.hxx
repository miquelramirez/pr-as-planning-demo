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

#ifndef __WALK_TO__
#define __WALK_TO__

#include <QObject>
#include <strips_prob.hxx>
#include <fluent.hxx>
#include <action.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;
using aptk::Fluent_Ptr_Vec;
using aptk::Action;

namespace Application
{

class Door;
class Room;

class WalkTo : public QObject
{
	Q_OBJECT
public:
	WalkTo( Room* src, Room* dst, Door* d );
	virtual ~WalkTo();

	bool		checkPrecondition();

	const QString&	sourceName() const;
	const QString& 	destName() const;

	void		makeSTRIPSAction( STRIPS_Problem& p );

signals:

	void	executed( unsigned index ); 

public slots:
	void	execute();
	
protected:

	Room*			mSource;
	Room*			mDestination;
	Door*			mDoor;
	Action*			mSTRIPSAction;
	Fluent_Ptr_Vec		mPrecFluents;
	Fluent_Ptr_Vec		mAddFluents;
	Fluent_Ptr_Vec		mDelFluents;
		
};


}

#endif
