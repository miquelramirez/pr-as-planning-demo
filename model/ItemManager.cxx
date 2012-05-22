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
