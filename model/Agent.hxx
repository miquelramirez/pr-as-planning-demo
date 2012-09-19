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

#ifndef __AGENT__
#define __AGENT__

#include <QObject>
#include <QMap>
#include <QList>
#include <strips_prob.hxx>
#include <fluent.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;

namespace Application
{

class Item;

class Agent : public QObject
{
	Q_OBJECT
public:
	static	Agent&	instance();
	virtual ~Agent();

	bool				canWalk() 	{ return mCanWalk; }
	Fluent*				canWalkFluent() { return mCanWalkFluent; }

	QList<Item*>&			inventory() 	  { return mInventory; }
	QMap<QString, Fluent*>&		carryingFluents() { return mCarryingFluents; } 
	
	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				evalSTRIPSFluents( aptk::Fluent_Vec& eval );

public slots:

	void	sitDown();
	void	lieOn();
	void	getUp();
	void	addItem( Item* i );
	void	removeItem( Item* i );
protected:

	Agent();

protected:
	bool				mCanWalk;
	Fluent*				mCanWalkFluent;
	QList< Item* >			mInventory;
	QMap< QString, Fluent* >	mCarryingFluents;
};

}

#endif // Agent.hxx
