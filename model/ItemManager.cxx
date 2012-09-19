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

#include <model/ItemManager.hxx>
#include <model/Item.hxx>

namespace Application
{

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

ItemManager&	ItemManager::instance()
{
	static ItemManager theItem;
	return theItem;
}

Item*	ItemManager::getItem( const QString& item )
{
	return mItemTable[item];
}

bool	ItemManager::loadItems( const QDomElement& root )
{
	QDomElement itemListElem = root.firstChildElement( "Items" );
	
	if ( itemListElem.isNull() ) return true;

	QDomElement itemElem = itemListElem.firstChildElement();
	while ( !itemElem.isNull() )
	{
		if ( itemElem.tagName() != "Item" ) continue;

		Item* i = new Item;
		if ( !Item::load( itemElem, i ) )
			return false;
		
		mItemTable[i->name()] = i;
		mItemList.push_back( i );	

		itemElem = itemElem.nextSiblingElement();
	}
	return true;	
}

}
