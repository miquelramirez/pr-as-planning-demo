#ifndef __TURN_ON_ACTION__
#define __TURN_ON_ACTION__

#include <model/StagePropAction.hxx>

namespace Application
{

class	GetOff : public StagePropAction
{

public:

	GetOff( Agent* a, Room* r, StageProp* obj );
	virtual ~GetOff();

	// StagePropAction interface
	virtual bool	checkPrecondition();
	virtual void	makeSTRIPSAction( STRIPS_Problem& p );
	virtual void	execute();

protected:

};

}


#endif // GetOff.hxx
