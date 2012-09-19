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

#ifndef __ITEM__
#define __ITEM__

#include <QObject>
#include <QString>
#include <QDomElement>

namespace Application
{

class Item : public QObject
{
	Q_OBJECT
public:

	Item();
	virtual ~Item();

	static	bool	load( const QDomElement& elem, Item* obj );

	const QString&	name() const { return mName; }
	const QString&	initialLocation() const { return mInitialLocation; }

protected:

	QString		mName;
	QString		mInitialLocation;
};

}

#endif // Item.hxx
