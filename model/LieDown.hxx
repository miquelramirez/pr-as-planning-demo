#ifndef __LIE_DOWN_ACTION__
#define __LIE_DOWN_ACTION__

#include <model/StagePropAction.hxx>

namespace Application
{

class	LieDown : public StagePropAction
{

public:

	LieDown( Agent* a, Room* r, StageProp* obj );
	virtual ~LieDown();

	// StagePropAction interface
	virtual bool	checkPrecondition();
	virtual void	makeSTRIPSAction( STRIPS_Problem& p );
	virtual void	execute();

protected:

};

}


#endif // LieDown.hxx
