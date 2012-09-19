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

#ifndef __STAGE_PROP__
#define __STAGE_PROP__

#include <QObject>
#include <QDomElement>
#include <QMap>
#include <QString>
#include <QVector>
#include <strips_prob.hxx>
#include <fluent.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;

namespace Planning
{
class Observer;
}

namespace Application
{

class	Room;
class	StagePropAction;
class	Agent;

class	StageProp : public QObject
{
	Q_OBJECT
public:
	StageProp();
	virtual ~StageProp();

	bool				getProperty( QString key );
	Fluent*				getPropertyFluent( QString key );

	QMap< QString, StagePropAction* >&	actions() { return mActions; }
	void					setRoom( Room* r );
	QString					name() { return mName; }
	QString					locationName() { return mLocName; }

	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* obs );
	void				evalSTRIPSFluents( aptk::Fluent_Vec& eval );

	static	bool			load( const QDomElement& elem, StageProp* obj );

public slots:

	void				setProperty( QString key, bool value );

protected:
	QString				mName;
	QMap< QString, bool>		mProperties;
	QMap< QString, Fluent*> 	mPropertiesFluents;
	QMap< QString, StagePropAction* >	mActions;
	QVector< QString >		mActionNames;
	QString				mLocName;
	Room*				mLocation;
};


}

#endif // StageProp.hxx
