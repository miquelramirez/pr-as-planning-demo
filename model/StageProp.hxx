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
