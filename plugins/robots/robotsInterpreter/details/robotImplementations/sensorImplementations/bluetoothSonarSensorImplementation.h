#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothSonarSensorImplementation : public BluetoothSensorImplementation
{
	Q_OBJECT
public:
	BluetoothSonarSensorImplementation(RobotCommunicator *robotCommunicationInterface
			, inputPort::InputPortEnum const &port);
	virtual void read();

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};

}
}
}
}
}
}
