#ifndef __AGENT__
#define __AGENT__

#include <QObject>
#include <QMap>
#include <QList>
#include <strips_prob.hxx>
#include <fluent.hxx>

using aptk::STRIPS_Problem;
using aptk::Fluent;

namespace Application
{

class Item;

class Agent : public QObject
{
	Q_OBJECT
public:
	static	Agent&	instance();
	virtual ~Agent();

	bool				canWalk() 	{ return mCanWalk; }
	Fluent*				canWalkFluent() { return mCanWalkFluent; }

	QList<Item*>&			inventory() 	  { return mInventory; }
	QMap<QString, Fluent*>&		carryingFluents() { return mCarryingFluents; } 
	
	void				makeSTRIPSFluents( STRIPS_Problem& domain );
	void				evalSTRIPSFluents( aptk::Fluent_Vec& eval );

public slots:

	void	sitDown();
	void	lieOn();
	void	getUp();
	void	addItem( Item* i );
	void	removeItem( Item* i );
protected:

	Agent();

protected:
	bool				mCanWalk;
	Fluent*				mCanWalkFluent;
	QList< Item* >			mInventory;
	QMap< QString, Fluent* >	mCarryingFluents;
};

}

#endif // Agent.hxx
