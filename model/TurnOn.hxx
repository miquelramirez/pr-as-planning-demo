#ifndef __TURN_ON_ACTION__
#define __TURN_ON_ACTION__

#include <model/StagePropAction.hxx>

namespace Application
{

class	TurnOn : public StagePropAction
{

public:

	TurnOn( Agent* a, Room* r, StageProp* obj );
	virtual ~TurnOn();

	// StagePropAction interface
	virtual bool	checkPrecondition();
	virtual void	makeSTRIPSAction( STRIPS_Problem& p );
	virtual void	execute();

protected:

};

}


#endif // TurnOn.hxx
