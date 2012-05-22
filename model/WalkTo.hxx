#ifndef __WALK_TO__
#define __WALK_TO__

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

class WalkTo : public QObject
{
	Q_OBJECT
public:
	WalkTo( Room* src, Room* dst, Door* d );
	virtual ~WalkTo();

	bool		checkPrecondition();

	const QString&	sourceName() const;
	const QString& 	destName() const;

	void		makeSTRIPSAction( STRIPS_Problem& p );

signals:

	void	executed( unsigned index ); 

public slots:
	void	execute();
	
protected:

	Room*			mSource;
	Room*			mDestination;
	Door*			mDoor;
	Action*			mSTRIPSAction;
	Fluent_Ptr_Vec		mPrecFluents;
	Fluent_Ptr_Vec		mAddFluents;
	Fluent_Ptr_Vec		mDelFluents;
		
};


}

#endif
