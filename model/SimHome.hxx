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

#ifndef __SIM_HOME__
#define __SIM_HOME__

#include <QObject>
#include <QString>
#include <QPixmap>
#include <strips_prob.hxx>

namespace Planning
{

class Observer;

}

namespace UI
{

class RoomView;

}

namespace Application
{

class 	Home;
class	Goal;

class SimHome : public QObject
{
	Q_OBJECT
public:

	SimHome();
	~SimHome();

	aptk::STRIPS_Problem&		planningDomain() { return mDomain; }
	Planning::Observer*		observer() { return mObserver; }
	
signals:
	
	void	onHomeLoaded( const QPixmap& pic );
	void	makeRoom( UI::RoomView*	v );
	void	updateInitialState( aptk::Fluent_Vec& eval );

public slots:

	void	quit();
	void	loadHome( const QString& homePath );
	void	initialStateUpdateRequested();

private:

	Home*				mHome;
	aptk::STRIPS_Problem		mDomain;
	Planning::Observer*		mObserver;
};

}

#endif // SimHome.hxx
