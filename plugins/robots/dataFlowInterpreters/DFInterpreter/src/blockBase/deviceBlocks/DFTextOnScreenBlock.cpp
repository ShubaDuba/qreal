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

#include "DFTextOnScreenBlock.h"

using namespace dataFlowBlocks::details;

DFTextOnScreenBlock::DFTextOnScreenBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: DisplayDevice(robotModel)
{
	portAssociatedWithProperty["CF_OUT"] = 0;
	portAssociatedWithProperty["COORDS"] = 1;
	portAssociatedWithProperty["TEXT"] = 2;
}

int DFTextOnScreenBlock::activationPortNumber() const
{
	return portAssociatedWithProperty["TEXT"];
}

void DFTextOnScreenBlock::init()
{
	mText = stringProperty("text");
	mRedraw = boolProperty("Redraw");
	mEvaluate = boolProperty("Evaluate");
}

void DFTextOnScreenBlock::handleData(Display &display)
{
	if (hasNewData("TEXT")) {
		QString rawText = propertyFromPort("TEXT").toString();
		mText = mEvaluate ? evalCode<QString>(rawText) : rawText;
	}

	if (hasNewData("COORDS")) {
		QVariantList coords = propertyFromPort("COORDS").value<QVariantList>();
		if (coords.size() != 2) {
			error("coords[] size should be equal to 2");
			return;
		}

		int x = coords.at(0).toInt();
		int y = coords.at(1).toInt();

		display.printText(x, y, mText);
		if (mRedraw) {
			display.redraw();
		}

		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
	}
}
