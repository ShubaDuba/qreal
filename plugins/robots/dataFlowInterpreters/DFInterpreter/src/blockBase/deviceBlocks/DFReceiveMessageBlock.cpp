/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "DFReceiveMessageBlock.h"

#include <QtWidgets/QApplication>

using namespace dataFlowBlocks::details;

DFReceiveMessageBlock::DFReceiveMessageBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: ShellDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
}

void DFReceiveMessageBlock::handleData(Shell &shell)
{
	if (hasNewData("CF_IN")) {
		propertyFromPort("CF_IN");
		disconnect(&shell, &Shell::mailArrived, this, &DFReceiveMessageBlock::handleMail);
		connect(&shell, &Shell::mailArrived, this, &DFReceiveMessageBlock::handleMail, Qt::QueuedConnection);
	}
}

void DFReceiveMessageBlock::handleMail(const QString &text)
{
	QStringList textList = text.split(" contents: ");

	QString sender = textList[0];
	textList.pop_front();

	int senderNum = sender.mid(QString("sender: ").length()).toInt();

	QString msg = textList[0];
	textList.pop_front();

	if (textList.length() > 0) {
		for (const QString str : textList) {
			msg += str;
		}
	}

	QVariantList res = {QVariant(senderNum), QVariant(msg)};

	emit newDataInFlow(res, portAssociatedWithProperty["OUT"]);
}

