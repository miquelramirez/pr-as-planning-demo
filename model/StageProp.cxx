#include <model/StageProp.hxx>
#include <model/StagePropAction.hxx>
#include <model/StagePropActionFactory.hxx>
#include <model/Room.hxx>
#include <planning/Observer.hxx>
#include <QApplication>
#include <QMessageBox>

namespace Application
{

StageProp::StageProp()
	: mName( "" )
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

bool	StageProp::load(  const QDomElement& root, StageProp* obj )
{
	obj->mName = root.attribute("name");
	QDomElement propElem = root.firstChildElement( "Properties" );
	assert( !propElem.isNull() );
	QDomElement propElemChild = propElem.firstChildElement();
	while ( !propElemChild.isNull() )
	{
		if ( propElemChild.tagName() != "Property" ) continue;
	
		mProperties[ propElemChild.attribute("name") ] = false;	

		propElemChild = propElemChild.nextSiblingElement();
	}
	assert( !mProperties.empty() );

	QDomElement actElem = root.firstChildElement( "Actions" );
	assert( !actElem.isNull() );
	QDomElement actElemChild = actElem.firstChildElement();
	while ( !actElemChild.isNull() )
	{
		if ( actElemChild.tagName() != "Action" ) continue;

		mActionNames.push_back( actElemChild.attribute("name") );	

		actElemChild = actElemChild.nextSiblingElement();
	}
	assert( !mActionNames.empty() );
}

void	StageProp::makeSTRIPSFluents( STRIPS_Problem& domain )
{
}

void	StageProp::makeSTRIPSActions( STRIPS_Problem& domain )
{
}

void	StageProp::evalSTRIPSFluents( aig_tk::Fluent_Vec& eval )
{
}

void	StageProp::setRoom( Room* r )
{

}

}
