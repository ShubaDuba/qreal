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

#include "DFInitCameraBlock.h"


using namespace dataFlowBlocks::details;

DFInitCameraBlock::DFInitCameraBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: LineSensorDevice(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["TYPE"] = 2;
}

void DFInitCameraBlock::init()
{
	mCameraMode = stringProperty("type");
}

void DFInitCameraBlock::handleData(dataFlowBlocks::details::LineSensor &lineSensor)
{
	if (hasNewData("CF_IN") && mCameraMode == "Line") {
		lineSensor.init();
	} else if (hasNewData("TYPE")) {
		if (propertyFromPort("TYPE").toString() != "Line") {
			error("other modes aren't available now");
			return;
		} else {
			lineSensor.init();
		}
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
}
