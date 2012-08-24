#ifndef __PICKUP__
#define __PICKUP__

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
