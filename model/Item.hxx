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
