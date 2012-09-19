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

#include <model/Item.hxx>
#include <QMessageBox>
#include <QApplication>

namespace Application
{

Item::Item()
{
}

Item::~Item()
{
}

bool	Item::load( const QDomElement& root, Item* obj )
{
	if (!root.hasAttribute("name"))
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Found item without 'name' attribute"));
		return false;	
	}
	obj->mName = root.attribute("name");
	QDomElement elem = root.firstChildElement("Location");
	if ( elem.isNull() )
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Item '%1' doesn't specify initial <Location>").arg(obj->name()));
		return false;	
	}
	if (!elem.hasAttribute("name"))
	{
		QMessageBox::information( QApplication::activeWindow(), 
			tr("Sim Home"),
			tr("Item '%1' initial <Location> has no name!").arg(obj->name()));
		return false;	
	}
	obj->mInitialLocation = elem.attribute("name");
	return true;
}

}
