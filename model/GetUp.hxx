#ifndef __GET_UP_ACTION__
#define __GET_UP_ACTION__

#include <model/StagePropAction.hxx>

namespace Application
{

class	GetUp : public StagePropAction
{

public:

	GetUp( Agent* a, Room* r, StageProp* obj );
	virtual ~GetUp();

	// StagePropAction interface
	virtual bool	checkPrecondition();
	virtual void	makeSTRIPSAction( STRIPS_Problem& p );
	virtual void	execute();

protected:

};

}


#endif // GetUp.hxx
