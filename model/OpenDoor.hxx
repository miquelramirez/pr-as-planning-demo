#ifndef __OPEN_DOOR__
#define __OPEN_DOOR__

#include <QObject>
#include <planning/STRIPS_Problem.hxx>
#include <planning/Fluent.hxx>
#include <planning/Action.hxx>

using aig_tk::STRIPS_Problem;
using aig_tk::Fluent;
using aig_tk::Fluent_Ptr_Vec;
using aig_tk::Action;

namespace Application
{

class Door;
class Room;

class OpenDoor : public QObject
{
	Q_OBJECT
public:
	OpenDoor( Door* d, Room* r );
	virtual ~OpenDoor();

	bool		checkPrecondition();
	
	const QString&	doorName() const;
	const QString&	roomName() const;
	
	void		makeSTRIPSAction( STRIPS_Problem& p );

signals:
	
	void	executed( unsigned index );

public	slots:

	void	execute();

protected:
	Door*			mDoor;
	Room*			mRoom;
	Action*			mSTRIPSAction;
	Fluent_Ptr_Vec		mPrecFluents;
	Fluent_Ptr_Vec		mAddFluents;
	Fluent_Ptr_Vec		mDelFluents;

};

}


#endif // OpenDoor.hxx
