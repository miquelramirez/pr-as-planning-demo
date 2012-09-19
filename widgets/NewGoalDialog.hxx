/*
Plan Recognition as Planning Demo: Home domain
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
