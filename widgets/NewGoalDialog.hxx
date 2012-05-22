#ifndef __NEW_GOAL_DIALOG__
#define __NEW_GOAL_DIALOG__

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QListView>
#include <QDialogButtonBox>
#include <QStringListModel>

namespace UI
{

class NewGoalDialog : public QDialog
{
	Q_OBJECT

public:

	NewGoalDialog( const QString& title, QList< QString >& fluents, QList< QString >& goals, QWidget* parent = NULL );
	virtual ~NewGoalDialog();

	QString			goalName() { return mGoalNameInput->text(); }
	QList< QString >&	fluentsInGoal() { return mFluentsInGoalList; }

public slots:

	void	verify();

	void	onAddFluent();
	void	onRemoveFluent();
	void	onClearFluent();

protected:

	void	setupWidgets();
	void	setupGoalNameGroup();
	void	setupFluentsGroup();
	void	setupFluentsControlGroup();

protected:
	QVBoxLayout*		mDialogLayout;
	QWidget*		mGoalNameGroup;
	QVBoxLayout*		mGoalNameLayout;
	QLabel*			mGoalNameLabel;
	QLineEdit*		mGoalNameInput;
	
	QWidget*		mFluentsWidget;
	QHBoxLayout*		mFluentsWidgetLayout;
	QWidget*		mAvailableFluentsGroup;
	QVBoxLayout*		mAvailableFluentsLayout;
	QLabel*			mAvailableFluentsLabel;
	QListView*		mAvailableFluents;
	QWidget*		mFluentsInGoalGroup;
	QVBoxLayout*		mFluentsInGoalLayout;
	QLabel*			mFluentsInGoalLabel;
	QListView*		mFluentsInGoal;

	QWidget*		mFluentsControlWidget;
	QHBoxLayout*		mFluentsControlLayout;
	QPushButton*		mAddFluent;
	QPushButton*		mRemoveFluent;
	QPushButton*		mClearFluents;

	QDialogButtonBox*	mDialogButtonBox;
	QStringListModel	mFluentsModel;
	QStringListModel	mFluentsInGoalModel;
	QStringList		mFluentsInGoalList;
	QList< QString >	mExistingGoals;
};

}

#endif // NewGoalDialog.hxx
