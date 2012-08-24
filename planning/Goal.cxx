#include <planning/Goal.hxx>
#include <fluent.hxx>

namespace Planning
{

Goal::Goal( STRIPS_Problem& p, Fluent_Vec& fluents )
	: mDomain( p ), mGoalFluents( fluents ), mProbability( 0.0f )
{
	makeFluentsNameList();	
}

void Goal::makeFluentsNameList()
{
	for ( unsigned k = 0; k < mGoalFluents.size(); k++ )
		mFluentNames.push_back( mDomain.fluents()[mGoalFluents[k]]->signature().c_str() );	
}

Goal::~Goal()
{
}

void	Goal::setProbability( float v )
{
	mProbability = v;
	emit 	probabilityChanged( v );
}

}
