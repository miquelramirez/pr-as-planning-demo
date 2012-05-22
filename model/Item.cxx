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
