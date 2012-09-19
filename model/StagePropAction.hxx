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

#ifndef __STAGE_PROP_ACTION__
#define __STAGE_PROP_ACTION__

#include <QObject>
#include <QMap>
#include <strips_prob.hxx>
#include <fluent.hxx>
#include <action.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;
using aptk::Fluent_Ptr_Vec;
using aptk::Action;

namespace Application
{

class Agent;
class Room;
class StageProp;

class	StagePropAction : public QObject
{
	Q_OBJECT
public:

	StagePropAction( Agent* a, Room* r, StageProp* obj );
	virtual ~StagePropAction();

	QString	roomName() const;
	QString	objName() const;

	virtual bool	checkPrecondition() = 0;

	virtual void 	makeSTRIPSAction( STRIPS_Problem& p ) = 0;

signals:

	void	executed( unsigned index );

public slots:

	virtual	void	execute() = 0;

protected:

	void			setSTRIPSAction( Action* a );
	Action&			stripsAction();

	Agent&			agentRef();
	Room&			roomRef();
	StageProp&		objRef();
	Fluent_Ptr_Vec&		precs() { return mPrecFluents; }
	Fluent_Ptr_Vec&		adds() { return mAddFluents; }
	Fluent_Ptr_Vec& 	dels() { return mDelFluents; }

protected:

	Agent*			mAgent;
	Room*			mRoom;
	StageProp*		mObject;
	Action*			mSTRIPSAction;
	Fluent_Ptr_Vec		mPrecFluents;
	Fluent_Ptr_Vec		mAddFluents;
	Fluent_Ptr_Vec		mDelFluents;
	

	
};

class 	StagePropActionCreator
{
public:
	virtual ~StagePropActionCreator();
	virtual StagePropAction*	operator()(Agent* a, Room* r, StageProp* obj) = 0;
};

class	StagePropActionFactory
{
public:

	~StagePropActionFactory();
	
	static StagePropActionFactory&	instance();

	StagePropAction*	produce( QString prodKey, Agent* a, Room* r, StageProp* obj );

	void			registerProduct( QString prodKey, StagePropActionCreator* creator );

protected:

	StagePropActionFactory();
	
protected:

	QMap< QString, StagePropActionCreator* >	mProductCreators;

};

template < typename ProductType >
class StagePropActionFactoryRegistrar
{
public:

	template < typename T >
	class ProductCreator : public StagePropActionCreator
	{
	public:
		virtual StagePropAction* operator()(Agent* a, Room* r, StageProp* obj)
		{
			return new T(a, r, obj);
		}
	};


	StagePropActionFactoryRegistrar(QString productName) 
	{
		StagePropActionFactory::instance().registerProduct( productName, new ProductCreator<ProductType>() );
	}

};

}

#endif
