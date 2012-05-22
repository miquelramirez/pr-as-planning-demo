#ifndef __ITEM_MANAGER__
#define __ITEM_MANAGER__

#include <QList>
#include <QMap>
#include <QDomElement>
#include <model/Item.hxx>

namespace Application
{

class ItemManager
{
public:

	static	ItemManager&	instance();
	~ItemManager();

	QList<Item*>&		items() { return mItemList; }
	Item*			getItem( const QString& item );
	bool			loadItems( const QDomElement& root );

protected:

	ItemManager();

protected:

	QList< Item* >		mItemList;
	QMap< QString, Item* >	mItemTable;
};

}

#endif // ItemManager.hxx
