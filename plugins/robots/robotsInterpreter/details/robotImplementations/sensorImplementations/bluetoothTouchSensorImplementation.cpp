#include "bluetoothTouchSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothTouchSensorImplementation::BluetoothTouchSensorImplementation(RobotCommunicator *robotCommunicationInterface
		, inputPort::InputPortEnum const &port)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType::touchBoolean, lowLevelSensorType::SWITCH, sensorMode::RAWMODE, port)
{
}

void BluetoothTouchSensorImplementation::read()
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

	// not implemented
}

void BluetoothTouchSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	Q_UNUSED(reading)
}
