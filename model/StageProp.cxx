#include <model/StageProp.hxx>
#include <model/StagePropAction.hxx>
#include <model/Room.hxx>
#include <model/Agent.hxx>
#include <planning/Observer.hxx>
#include <QApplication>
#include <QMessageBox>

namespace Application
{

StageProp::StageProp()
	: mName( "" ), mLocation( NULL )
{
}

StageProp::~StageProp()
{
}

void	StageProp::setProperty( QString key, bool value )
{
	mProperties[key] = value;
}

bool	StageProp::getProperty( QString key )
{
	assert( mProperties.find(key) != mProperties.end() );
	return mProperties[key];
}

Fluent*	StageProp::getPropertyFluent( QString key )
{
	assert( mPropertiesFluents.find(key) != mPropertiesFluents.end() );
	return mPropertiesFluents[key];
}

bool	StageProp::load(  const QDomElement& root, StageProp* obj )
{
	obj->mName = root.attribute("name");

	QDomElement locElem = root.firstChildElement( "Location" );
	assert( !locElem.isNull() );

	obj->mLocName = locElem.attribute( "name" );

	QDomElement propElem = root.firstChildElement( "Properties" );
	assert( !propElem.isNull() );
	QDomElement propElemChild = propElem.firstChildElement();
	while ( !propElemChild.isNull() )
	{
		if ( propElemChild.tagName() != "Property" ) continue;

		bool initValue = ( propElemChild.attribute("initial").toInt() ? true : false );
	
		obj->mProperties[ propElemChild.attribute("name") ] = initValue;	

		propElemChild = propElemChild.nextSiblingElement();
	}
	assert( !obj->mProperties.empty() );

	QDomElement actElem = root.firstChildElement( "Actions" );
	assert( !actElem.isNull() );
	QDomElement actElemChild = actElem.firstChildElement();
	while ( !actElemChild.isNull() )
	{
		if ( actElemChild.tagName() != "Action" ) continue;

		obj->mActionNames.push_back( actElemChild.attribute("name") );	

		actElemChild = actElemChild.nextSiblingElement();
	}
	assert( !obj->mActionNames.empty() );
	return true;
}

void	StageProp::makeSTRIPSFluents( STRIPS_Problem& domain )
{
	typedef QMap< QString, bool >::iterator Iterator;

	for ( Iterator i = mProperties.begin(); i != mProperties.end(); i++ ) 
	{
		QString tmp( "(%1 %2)" );
		unsigned index = STRIPS_Problem::add_fluent( domain, tmp.arg( i.key() ).arg( name() ).toStdString() );	
		mPropertiesFluents[ i.key() ] = domain.fluents()[ index ];
	}
}

void	StageProp::makeSTRIPSActions( STRIPS_Problem& domain, Planning::Observer* o )
{
	Agent& agent = Agent::instance();
	StagePropActionFactory& actionFactory = StagePropActionFactory::instance();
	
	for ( int k = 0; k < mActionNames.size(); k++ )
	{
		StagePropAction* a = actionFactory.produce( mActionNames[k], &agent, mLocation, this );
		a->makeSTRIPSAction(domain);
		mActions[mActionNames[k]] = a ;
		QObject::connect( a, SIGNAL( executed(unsigned) ), o, SLOT(actionExecuted(unsigned)) );
	}	
}

void	StageProp::evalSTRIPSFluents( aig_tk::Fluent_Vec& eval )
{
	typedef QMap< QString, bool >::iterator Iterator;

	for ( Iterator i = mProperties.begin(); i != mProperties.end(); i++ ) 
		if ( i.value() )
			eval.push_back( mPropertiesFluents[i.key()]->index() );
}

void	StageProp::setRoom( Room* r )
{
	mLocation = r;
}

}
