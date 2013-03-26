#include "bluetoothSonarSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothSonarSensorImplementation::BluetoothSonarSensorImplementation(RobotCommunicator *robotCommunicationInterface
		, inputPort::InputPortEnum const &port)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType::sonar, lowLevelSensorType::LOWSPEED_9V, sensorMode::RAWMODE, port)
{
}

void BluetoothSonarSensorImplementation::read()
{
	if (!mIsConfigured) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

	if (mState == pending) {
		return;
	}
	mState = pending;

	QString const command = QString("sonar_sensor %1\n").arg(portString());
	mRobotCommunicationInterface->send(this, command.toLatin1(), 2);
}

void BluetoothSonarSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
}
