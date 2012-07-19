#ifndef __SIT_DOWN_ACTION__
#define __SIT_DOWN_ACTION__

#include <model/StagePropAction.hxx>

namespace Application
{

class	SitDown : public StagePropAction
{

public:

	SitDown( Agent* a, Room* r, StageProp* obj );
	virtual ~SitDown();

	// StagePropAction interface
	virtual bool	checkPrecondition();
	virtual void	makeSTRIPSAction( STRIPS_Problem& p );
	virtual void	execute();

protected:

};

}


#endif // SitDown.hxx
