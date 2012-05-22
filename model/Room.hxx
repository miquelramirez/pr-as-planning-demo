#ifndef __ROOM__
#define __ROOM__

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QDomElement>
#include <QMenu>
#include <QList>
#include <QMap>
#include <planning/STRIPS_Problem.hxx>
#include <planning/Fluent.hxx>

using aig_tk::STRIPS_Problem;
using aig_tk::Fluent;

namespace Planning
{
class Observer;
}

namespace Application
{

class Item;
class Door;
class WalkTo;
class Drop;
class PickUp;

class Room : public QObject
{
	Q_OBJECT
public:

	Room( );
	virtual ~Room();

	static bool 			load( const QDomElement& elem, Room* obj );

	const QString& 			name() const { return mName; }
	const QPoint&			center() const { return mCenter; }
	const QVector< QPoint >&	geometry() const { return mGeometry; }

	QVector< Room* >&		neighbors()  { return mConnections; }
	const QVector< QString >&	neighborNames() const { return mConnPlaceholders; }
	QVector< Door* >&		doors() { return mDoors; }
	const QVector< QString >&	doorNames() const { return mDoorPlaceholders; }

	QVector< WalkTo* >&		walkToActions() { return mWalkToActions; }
	void				connectTo( Room* other, Door* );

	QList< Item*>&			contents() { return mContents; }
	QVector< PickUp* >&		pickUpActions() { return mPickUpActions; }
	QVector< Drop* >&		dropActions() { return mDropActions; }
	void				makeItemActions();

	void				setAgentLocation( bool v );
	bool				isAgentHere() const { return mAgentHere; }
	
	QMenu*				getActionMenu();	

	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs );
	void				evalSTRIPSFluents( aig_tk::Fluent_Vec& eval );

	Fluent*				atFluent() 		{ return mAtFluent; }
	QMap< QString, Fluent* >&	atItemFluents() 	{ return mAtItemFluents; }

signals:
	
	void		showAgent();
	void		hideAgent();

public	slots:
	void		addItem( Item* i );
	void		removeItem( Item* i );

protected:

	QString				mName;
	QVector<QPoint>			mGeometry;
	QVector< QString >		mConnPlaceholders;
	QVector< QString >		mDoorPlaceholders;
	QVector< Room* >		mConnections;
	QVector< Door* >		mDoors;
	QPoint				mCenter;
	bool				mAgentHere;
	QMenu*				mActionMenu;
	QVector< WalkTo* >		mWalkToActions;
	QList< Item*>			mContents;
	QVector< PickUp* >		mPickUpActions;
	QVector< Drop* >		mDropActions;
	Fluent*				mAtFluent;
	QMap< QString, Fluent* >	mAtItemFluents;
};

}

#endif // Room.hxx
