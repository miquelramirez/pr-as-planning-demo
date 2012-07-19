#ifndef __HOME__
#define __HOME__

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QDomElement>
#include <vector>
#include <QMap>

namespace Application
{

class Room;
class Door;
class StageProp;

class	Home	: public QObject
{
	Q_OBJECT
public:
	
	Home(  );
	virtual	~Home();

	const QString&			name() const 		{ return mName; }
	const QPixmap&			floorPlan() const  	{ return mFloorPlanImg; }
	std::vector<Room* >&		rooms() 		{ return mRooms; }
	std::vector<Door* >&		doors()			{ return mDoors; }
	std::vector<StageProp* >&	props()			{ return mProps; }
	Door*				getDoorByName( QString name ) { return mDoorsTable[name]; } 	

	static bool			load( const QString& xmlDocument, Home* obj );

protected:
	
	bool				connectRooms();
	void				placeItems();
	void				placeProps();
	void				loadFloorPlan( QDomElement& elem );

protected:
	QString				mName;
	std::vector< Room* >		mRooms;
	QMap< QString, Room* >		mRoomsTable;
	QPixmap				mFloorPlanImg;
	std::vector< Door* >		mDoors;
	QMap< QString, Door* >		mDoorsTable;
	std::vector< StageProp* >	mProps;
	QMap< QString, StageProp* >	mPropsTable;
 
};

}

#endif // Home.hxx
