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

#include <QApplication>
#include <iostream>
#include <model/SimHomeConfig.hxx>
#include <model/SimHome.hxx>
#include <widgets/AppWindow.hxx>
#include <cstdlib>
#include <planning/Observer.hxx>
#include <widgets/GoalSetView.hxx>
#include <QThread>

void	wireSignalsAndSlots( Application::SimHome& appObj, UI::AppWindow& win )
{
	QObject::connect( win.quitAction(), SIGNAL(triggered()), &appObj, SLOT(quit()) );
	QObject::connect( &win, SIGNAL( loadHome( const QString& ) ), &appObj, SLOT( loadHome( const QString& ) ) );
	QObject::connect( &appObj, SIGNAL( onHomeLoaded( const QPixmap& ) ), &win, SLOT( onHomeLoaded( const QPixmap& ) ) );
	QObject::connect( &appObj, SIGNAL( makeRoom( UI::RoomView* ) ), &win, SLOT( makeRoom( UI::RoomView* ) ) );
	QObject::connect( win.startObsAction(), SIGNAL( triggered() ), appObj.observer(), SLOT( start() ) );
	QObject::connect( win.stopObsAction(), SIGNAL( triggered() ), appObj.observer(), SLOT( stop() ) );
	QObject::connect( &win, SIGNAL( setObsLevel( float ) ), appObj.observer(), SLOT( setObsLevel( float ) ) );
	QObject::connect( win.goalSetView(), SIGNAL( requestHypGoalsInfo() ), appObj.observer(), SLOT( onHypGoalsInfoRequested() ) );
	QObject::connect( appObj.observer(), SIGNAL( sendHypGoalsInfo( QList< QString >&, QList< QString >& ) ),
				win.goalSetView(), SLOT( acceptHypGoalsInfo( QList< QString >&, QList< QString >& ) ) );
	QObject::connect( win.goalSetView(), SIGNAL( createNewGoal( QString, QList< QString >& ) ),
			appObj.observer(), SLOT( addNewGoal( QString, QList< QString >& ) ) );
	QObject::connect( appObj.observer(), SIGNAL( goalViewCreated( UI::GoalView* ) ),
				win.goalSetView(), SLOT( addNewGoalView( UI::GoalView* )  ) );
	QObject::connect( appObj.observer(), SIGNAL( actionObserved( QString ) ), 
				win.observationsView(), SLOT( logAction( QString ) ) );
}

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	std::cout << "Loading config: " << Application::SimHomeConfig::instance().xmlDocPath() << std::endl;
	Application::SimHomeConfig::instance().loadXmlConfig();
	std::cout << "Ideal # of threads:" << QThread::idealThreadCount() << std::endl;

	srand( 3873831 );

	Application::SimHome home;
	UI::AppWindow win;

	wireSignalsAndSlots( home, win );
	

	win.show();

	return app.exec();
}
