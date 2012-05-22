#ifndef __PICKUP__
#define __PICKUP__

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
class Agent;
class Room;
class Item;

class	PickUp : public QObject
{
	Q_OBJECT
public:
	PickUp( Agent* a, Room* r, Item* i );
	virtual ~PickUp();

	bool	checkPrecondition();

	const 	QString&	roomName() const;
	const	QString&	itemName() const;

	void	makeSTRIPSAction( STRIPS_Problem& p );

signals:

	void	executed( unsigned index );

public slots:

	void	execute();

protected:

	Agent*		mAgent;
	Room*		mRoom;
	Item*		mItem;
	Action*			mSTRIPSAction;
	Fluent_Ptr_Vec		mPrecFluents;
	Fluent_Ptr_Vec		mAddFluents;
	Fluent_Ptr_Vec		mDelFluents;
 
};

}

#endif // PickUp.hxx
