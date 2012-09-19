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

#ifndef __GOAL__
#define __GOAL__

#include <QObject>
#include <QStringList>
#include <QString>
#include <strips_prob.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent_Vec;

namespace Planning
{

class Goal : public QObject
{

	Q_OBJECT

public:

	Goal( STRIPS_Problem& p, Fluent_Vec& fluents );
	virtual ~Goal();

	Fluent_Vec&		fluents() { return mGoalFluents; }

	void			setName( const QString& l ) { mName = l; }
	const QString&		name() const { return mName; }
	const QStringList&	fluentNames() { return mFluentNames; }

protected:

	void	makeFluentsNameList();

signals:

	void			probabilityChanged( float v );

public slots:

	void			setProbability( float v );

protected:

	STRIPS_Problem&		mDomain;
	Fluent_Vec		mGoalFluents;
	float			mProbability;
	QString			mName;
	QStringList		mFluentNames;
};

}

#endif // Goal.hxx
