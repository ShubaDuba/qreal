#include "changeVersionWidget.h"
#include "ui_changeVersionWidget.h"
#include <QtCore/QString>
#include <QDebug>
#include <QListIterator>

ChangeVersionWidget::ChangeVersionWidget(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::ChangeVersionWidget)
{
	mUi->setupUi(this);
	mUi->label->hide();
	connect(mUi->pushButtonLoad, SIGNAL(clicked()), SLOT(getHash()));
	mUi->listWidgetForLog->setMinimumWidth(this->width() / 5);
	initDiffWidget();
}

ChangeVersionWidget::~ChangeVersionWidget()
{
	delete mUi;
}

void ChangeVersionWidget::getHash()
{
	QString hash = mUi->listWidgetForLog->currentItem()->data(Qt::UserRole).toString();
	emit swapTab();
	mUi->listWidgetForLog->clear();
	mUi->listWidgetForLog->deleteLater();
	emit loadVersion(hash);
}

void ChangeVersionWidget::showDiff(QListWidgetItem *item)
{
	QString firstHash = item->data(Qt::UserRole).toString();
	int row = mUi->listWidgetForLog->row(item);
	QListWidgetItem *item2 = mUi->listWidgetForLog->item(row + 1);
	if (mDiffWidget->layout() != NULL){
		clearLayout(mDiffWidget->layout());
	}
	if (item2 != NULL){
		emit showDiff(firstHash, item2->data(Qt::UserRole).toString(), mDiffWidget);
	} else {
		emit showDiff(firstHash, QString(), mDiffWidget);
	}
}

void ChangeVersionWidget::initDiffWidget()
{
	mDiffWidget = new QWidget();
	QGridLayout *mLayout = new QGridLayout(mDiffWidget);
	mLayout->setMargin(0);
	mDiffWidget->setLayout(mLayout);

	mDiffWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	mUi->horizontalLayout->addWidget(mDiffWidget, 1, Qt::AlignLeft);
}

void ChangeVersionWidget::updateLog(QList<QPair<QString , QString> > listLog) // hash & mainPart
{
	if (listLog.size() != 0){
		disconnect(mUi->listWidgetForLog, SIGNAL(itemClicked(QListWidgetItem*))
				, this, SLOT(showDiff(QListWidgetItem*)));
		connect(mUi->listWidgetForLog, SIGNAL(itemClicked(QListWidgetItem*))
				, this, SLOT(showDiff(QListWidgetItem*)),Qt::QueuedConnection);
		mUi->listWidgetForLog->clear();
		int number = 0;
		while (number < listLog.size()){
			QListWidgetItem *item = new QListWidgetItem;
			item->setData(Qt::UserRole,listLog.at(number).first);
			item->setText(listLog.at(number).second);
			mUi->listWidgetForLog->addItem(item);
			number++;
		}
		mUi->listWidgetForLog->item(0)->setSelected(true);
		showDiff(mUi->listWidgetForLog->item(0));
	} else {
		mUi->listWidgetForLog->clear();
		QString text = "There are not version of the project or project was not versioned. \n Or project isn't loaded";
		mUi->label->setText(text);
		mUi->label->showMaximized();
	}
}

void ChangeVersionWidget::clearLayout(QLayout *layout)
{
	QLayoutItem *item;
	while((item = layout->takeAt(0))){
		if (item->layout()) {
			clearLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}