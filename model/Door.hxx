#ifndef __DOOR__
#define __DOOR__

#include <QObject>
#include <QDomElement>
#include <QVector>
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

class	Room;
class	OpenDoor;
class	CloseDoor;

class	Door	: public QObject
{
	Q_OBJECT
public:
	Door();
	virtual ~Door();

	static bool 			load( const QDomElement& elem, Door* obj );

	const QString &			name() const { return mName; }
	void				addRoom( Room* r );

	QVector< OpenDoor* >&		openActions() { return mOpenActions; }
	QVector< CloseDoor* >&		closeActions() { return mCloseActions; }
	
	bool				isOpen() { return mOpen; }
	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs );
	void				evalSTRIPSFluents( aig_tk::Fluent_Vec& eval );
	Fluent*				openFluent() { return mOpenFluent; }
	Fluent*				closedFluent() { return mClosedFluent; }

public slots:

	void				open();
	void				close();

protected:
	QString				mName;
	std::vector< Room* >		mRooms;
	QVector< OpenDoor* >		mOpenActions;
	QVector< CloseDoor* >		mCloseActions;
	bool				mOpen;
	Fluent*				mOpenFluent;
	Fluent*				mClosedFluent;
		

};


}

#endif // Door.hxx
