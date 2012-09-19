/*
Plan Recognition as Planning Demo: Home domain
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
