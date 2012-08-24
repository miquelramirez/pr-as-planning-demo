#ifndef __WALK_TO__
#define __WALK_TO__

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
