#ifndef __GOAL__
#define __GOAL__

#include <QObject>
#include <QStringList>
#include <QString>
#include <planning/STRIPS_Problem.hxx>

using aig_tk::STRIPS_Problem;
using aig_tk::Fluent_Vec;

namespace Planning
{

class Goal : public QObject
{

	Q_OBJECT

public:

	Goal( STRIPS_Problem& p, Fluent_Vec& fluents );
	virtual ~Goal();

	Fluent_Vec&		fluents() { return mGoalFluents; }

	void			setName( const QString& l ) { mName = l; }
	const QString&		name() const { return mName; }
	const QStringList&	fluentNames() { return mFluentNames; }

protected:

	void	makeFluentsNameList();

signals:

	void			probabilityChanged( float v );

public slots:

	void			setProbability( float v );

protected:

	STRIPS_Problem&		mDomain;
	Fluent_Vec		mGoalFluents;
	float			mProbability;
	QString			mName;
	QStringList		mFluentNames;
};

}

#endif // Goal.hxx
