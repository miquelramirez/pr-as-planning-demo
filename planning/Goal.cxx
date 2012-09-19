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

#include <planning/Goal.hxx>
#include <fluent.hxx>

namespace Planning
{

Goal::Goal( STRIPS_Problem& p, Fluent_Vec& fluents )
	: mDomain( p ), mGoalFluents( fluents ), mProbability( 0.0f )
{
	makeFluentsNameList();	
}

void Goal::makeFluentsNameList()
{
	for ( unsigned k = 0; k < mGoalFluents.size(); k++ )
		mFluentNames.push_back( mDomain.fluents()[mGoalFluents[k]]->signature().c_str() );	
}

Goal::~Goal()
{
}

void	Goal::setProbability( float v )
{
	mProbability = v;
	emit 	probabilityChanged( v );
}

}
