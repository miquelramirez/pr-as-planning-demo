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

#ifndef UI_APPWINDOW_HXX
#define UI_APPWINDOW_HXX

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QPixmap>
#include <QGroupBox>

namespace UI
{

class  RoomView;
class  HomeRenderer;
class  GoalSetView;
class  ObservationsView;

class	AppWindow : public QMainWindow
{
	Q_OBJECT
public:
	AppWindow( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	virtual ~AppWindow();	

	QAction*		quitAction() 		{ return mQuitAction; }
	HomeRenderer*		homeScene()  		{ return mHomeScene; }
	QAction*		startObsAction() 	{ return mStartObsAction; }
	QAction*		stopObsAction()		{ return mStopObsAction; }
	GoalSetView*		goalSetView()		{ return mGoalsView; }
	ObservationsView*	observationsView()	{ return mObsActView; }

signals:
	void		loadHome( const QString& homeXmlPath );
	void		setObsLevel( float v );

public slots:
	
	void	onLoadHome();
	void	onHomeLoaded( const QPixmap& pic );
	void	makeRoom( UI::RoomView* view );
	void	setObsLevel10();
	void	setObsLevel30();
	void	setObsLevel50();
	void	setObsLevel70();
	void	setObsLevel100();

protected:

	void	setupWidgets();
	void	makeWidgets();
	void	makeWorldWidgets();
	void	makeObserverWidgets();
	void	makeFileMenu();
	void	makePRMenu();
	void	makeAboutMenu();

protected:
	// Widgets
	QAction* 		mLoadHomeAction;
	QAction* 		mQuitAction;
	QWidget* 		mCentralWidget;
	QWidget*		mWorldWidget;
	QWidget*		mObserverWidget;
	QWidget* 		mHorizontalLayoutWidget;
	QHBoxLayout* 		mHorizontalLayout;
	QGraphicsView*		mGraphicDisplay;
	QMenuBar* 		mAppMenuBar;
	QMenu*			mFileMenu;
	QMenu*	 		mAboutMenu;
	QStatusBar*		mStatusBar;
	HomeRenderer*   	mHomeScene;
	QGroupBox*		mFloorPlanWidget;
	QVBoxLayout*		mFloorPlanWidgetLayout;
	GoalSetView*		mGoalsView;
	QMenu*			mPRMenu;
	QAction*		mStartObsAction;
	QAction*		mStopObsAction;
	QMenu*			mObsLevelMenu;
	ObservationsView*	mObsActView;
};

}

#endif // UI_APPWINDOW_HXX
