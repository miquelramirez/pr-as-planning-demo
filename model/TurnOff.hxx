#ifndef __TURN_OFF_ACTIOFF__
#define __TURN_OFF_ACTIOFF__

#include <model/StagePropAction.hxx>

namespace Application
{

class	TurnOff : public StagePropAction
{

public:

	TurnOff( Agent* a, Room* r, StageProp* obj );
	virtual ~TurnOff();

	// StagePropAction interface
	virtual bool	checkPrecondition();
	virtual void	makeSTRIPSAction( STRIPS_Problem& p );
	virtual void	execute();

protected:

};

}


#endif // TurnOff.hxx
