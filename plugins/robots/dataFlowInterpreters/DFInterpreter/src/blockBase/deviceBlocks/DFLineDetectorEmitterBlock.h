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
#pragma once

#include "plugins/robots/dataFlowInterpreters/DFInterpreter/src/blockBase/DFDeviceBlock.h"

#include <trikKit/robotModel/parts/trikLineSensor.h>

namespace dataFlowBlocks {
namespace details {

using LineSensor = trik::robotModel::parts::TrikLineSensor;
using LineSensorDevice = DFDeviceBlock<LineSensor>;

class DFLineDetectorEmitterBlock : public LineSensorDevice
{
	Q_OBJECT

public:
	explicit DFLineDetectorEmitterBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	void init() override;
	void handleData(LineSensor &lineSensor) override;

private slots:
	void newVectorDataReceived(QVector<int> data);

private:
	LineSensor *mSensor = nullptr;
	QString mCameraMode = QString();
};

}
}