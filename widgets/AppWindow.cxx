#include <widgets/AppWindow.hxx>
#include <iostream>
#include <QFileDialog>
#include <QStringList>
#include <widgets/HomeRenderer.hxx>
#include <widgets/RoomView.hxx>
#include <widgets/GoalSetView.hxx>

namespace UI
{

AppWindow::AppWindow( QWidget* parent, Qt::WindowFlags flags )
	: QMainWindow( parent, flags )
{

	setupWidgets();
	mHomeScene = new HomeRenderer;
	mGraphicDisplay->setScene( mHomeScene );
}

AppWindow::~AppWindow()
{
}

void	AppWindow::makeRoom( RoomView* v )
{
	mHomeScene->addItem( v );
	mGraphicDisplay->invalidateScene();
}

void	AppWindow::setupWidgets()
{
	// Main window
	if ( objectName().isEmpty() )
		setObjectName( QString::fromUtf8("ApplicationWindow") );
	resize( 800, 600 );
       	setWindowTitle(QApplication::translate("ApplicationWindow", "Sim Home", 0, QApplication::UnicodeUTF8));

	makeWidgets();
 
        mAppMenuBar = new QMenuBar(this);
        mAppMenuBar->setObjectName(QString::fromUtf8("AppMenuBar"));
        mAppMenuBar->setGeometry(QRect(0, 0, 803, 25));

	makeFileMenu();
	makePRMenu();
	makeAboutMenu();

        mAppMenuBar->addAction(mFileMenu->menuAction());
	mAppMenuBar->addAction(mPRMenu->menuAction());
        mAppMenuBar->addAction(mAboutMenu->menuAction());
     
	setMenuBar(mAppMenuBar);

        mStatusBar = new QStatusBar(this);
        mStatusBar->setObjectName(QString::fromUtf8("mStatusBar"));
        setStatusBar(mStatusBar);

        QMetaObject::connectSlotsByName(this);
	QObject::connect( mLoadHomeAction, SIGNAL(triggered()), this, SLOT(onLoadHome()) );
}

void	AppWindow::makeWidgets()
{
	mCentralWidget = new QWidget(this);
        mCentralWidget->setObjectName(QString::fromUtf8("mCentralWidget"));

        mHorizontalLayout = new QHBoxLayout(mCentralWidget);
        mHorizontalLayout->setObjectName(QString::fromUtf8("HorizontalLayout"));
        mHorizontalLayout->setContentsMargins(5, 5, 5, 5);

	mFloorPlanWidget = new QGroupBox("Floor Plan", mCentralWidget);	
	mFloorPlanWidget->setObjectName( QString::fromUtf8("FloorPlanWidget"));

	mFloorPlanWidgetLayout = new QVBoxLayout(mFloorPlanWidget);
        mFloorPlanWidgetLayout->setObjectName(QString::fromUtf8("FloorPlanWidgetLayout"));
        mFloorPlanWidgetLayout->setContentsMargins(10, 10, 10, 10);

        mGraphicDisplay = new QGraphicsView(mFloorPlanWidget);
        mGraphicDisplay->setObjectName(QString::fromUtf8("GraphicDisplay"));
        mGraphicDisplay->setBaseSize(QSize(800, 600));

	mFloorPlanWidgetLayout->addWidget(mGraphicDisplay);

	mGoalsView = new GoalSetView( "Goals", mCentralWidget );
	mGoalsView->setObjectName( QString::fromUtf8("GoalsView" ) );	
	mGoalsView->setMinimumWidth( 300 );

        mHorizontalLayout->addWidget(mFloorPlanWidget);
	mHorizontalLayout->addWidget(mGoalsView);

        setCentralWidget(mCentralWidget);
}

void	AppWindow::makeFileMenu()
{
	mFileMenu = new QMenu(mAppMenuBar);
        mFileMenu->setObjectName(QString::fromUtf8("FileMenu"));
	mFileMenu->setTitle(QApplication::translate("ApplicationWindow", "File", 0, QApplication::UnicodeUTF8));

	mLoadHomeAction = new QAction(this);
        mLoadHomeAction->setObjectName(QString::fromUtf8("LoadHomeAction"));
	mLoadHomeAction->setText(QApplication::translate("ApplicationWindow", "Load Home...", 0, QApplication::UnicodeUTF8));

        mQuitAction = new QAction(this);
        mQuitAction->setObjectName(QString::fromUtf8("mQuitAction"));
	mQuitAction->setText(QApplication::translate("ApplicationWindow", "&Quit", 0, QApplication::UnicodeUTF8));

	mFileMenu->addAction(mLoadHomeAction);
        mFileMenu->addSeparator();
        mFileMenu->addAction(mQuitAction);

}

void	AppWindow::makePRMenu()
{
	mPRMenu = new QMenu( mAppMenuBar );
	mPRMenu->setObjectName( QString::fromUtf8("PRMenu"));
	mPRMenu->setTitle( QApplication::translate( "ApplicationWindow", "PR", 0, QApplication::UnicodeUTF8));

	mObsLevelMenu = new QMenu( mPRMenu );
	mObsLevelMenu->setObjectName( QString::fromUtf8("ObsLevelMenu") );
	mObsLevelMenu->setTitle( QApplication::translate( "ApplicationWindow", "Observation Level", 0, QApplication::UnicodeUTF8 ));

	QAction* obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel10") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "10%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel10() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel30") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "30%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel30() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel50") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "50%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel50() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel70") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "70%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel70() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	obsLvlAction = new QAction( this );
	obsLvlAction->setObjectName( QString::fromUtf8("ObsLevel100") );
	obsLvlAction->setText( QApplication::translate( "ApplicationWindow", "100%", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( obsLvlAction, SIGNAL( triggered()), this, SLOT( setObsLevel100() ) ); 
	mObsLevelMenu->addAction( obsLvlAction );

	mStartObsAction = new QAction( this );
	mStartObsAction->setObjectName( QString::fromUtf8("StartObserving") );
	mStartObsAction->setText( QApplication::translate("ApplicationWindow", "Start Observing...", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( mStartObsAction, SIGNAL( triggered() ), mGoalsView, SLOT( freeze() ) );

	mStopObsAction = new QAction( this );
	mStopObsAction->setObjectName( QString::fromUtf8("StopObserving") );
	mStopObsAction->setText( QApplication::translate("ApplicationWindow", "Stop Observing...", 0, QApplication::UnicodeUTF8 ));
	QObject::connect( mStopObsAction, SIGNAL( triggered() ), mGoalsView, SLOT( unFreeze() ) );

	mPRMenu->addMenu( mObsLevelMenu );
	mPRMenu->addAction( mStartObsAction );
	mPRMenu->addAction( mStopObsAction );
	
}

void	AppWindow::setObsLevel10()
{
	emit setObsLevel( 0.1f );
}

void	AppWindow::setObsLevel30()
{
	emit setObsLevel( 0.3f );
}

void	AppWindow::setObsLevel50()
{
	emit setObsLevel( 0.5f );
}

void	AppWindow::setObsLevel70()
{
	emit setObsLevel( 0.7f );
}

void	AppWindow::setObsLevel100()
{
	emit setObsLevel( 1.0f );
}

void	AppWindow::makeAboutMenu()
{
        mAboutMenu = new QMenu(mAppMenuBar);
        mAboutMenu->setObjectName(QString::fromUtf8("AboutMenu"));
	mAboutMenu->setTitle(QApplication::translate("ApplicationWindow", "About", 0, QApplication::UnicodeUTF8));
}

void	AppWindow::onLoadHome()
{
	QFileDialog dialog(this);

	dialog.setFileMode( QFileDialog::ExistingFile );
	dialog.setNameFilter(tr("XML documents (*.xml)"));	
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode( QFileDialog::AcceptOpen );

	QStringList filenames;

	if ( dialog.exec() )
	{
		filenames = dialog.selectedFiles();
		emit loadHome(filenames.first() );	
	}
		
}

void	AppWindow::onHomeLoaded( const QPixmap& pic )
{
	mHomeScene->setFloorPlan( pic );
	mGraphicDisplay->invalidateScene();
	mGoalsView->unFreeze();	
}

}
