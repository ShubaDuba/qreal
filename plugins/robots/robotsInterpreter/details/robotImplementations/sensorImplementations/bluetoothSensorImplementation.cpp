#include "bluetoothSensorImplementation.h"
#include "../../robotParts/robotModel.h"
#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothSensorImplementation::BluetoothSensorImplementation(RobotCommunicator *const robotCommunicationInterface
		, sensorType::SensorTypeEnum const &sensorType
		, lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, inputPort::InputPortEnum const &port)
	: AbstractSensorImplementation(port, sensorType)
	, mRobotCommunicationInterface(robotCommunicationInterface)
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
	, mIsConfigured(false)
	, mResetDone(false)
{
	connect(mRobotCommunicationInterface, SIGNAL(response(QObject*,QByteArray))
			, this, SLOT(readingDone(QObject*,QByteArray)));
}

BluetoothSensorImplementation::~BluetoothSensorImplementation()
{
}

void BluetoothSensorImplementation::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee != this)
		return;
	processResponse(reading);
}

void BluetoothSensorImplementation::processResponse(QByteArray const &reading)
{
	mState = idle;
	if (reading.isEmpty()) {
		if (mResetDone) {
			Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::processResponse", "Response is empty, seems to be a connection failure");
			// Just ignore connection failures for now
//			emit failure();
		}
	} else if (reading.length() == 2) {
		int const sensorValue = (static_cast<int>(reading[0]) << 8) + reading[1];
		Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::sensorSpecificProcessResponse", QString::number(sensorValue));
		emit response(sensorValue);
	}
}

void BluetoothSensorImplementation::configure()
{
	mIsConfigured = true;
	emit configured();
}

QString BluetoothSensorImplementation::portString() const
{
	// Even port = right, odd = left
	return mPort == inputPort::port1 || mPort == inputPort::port3 ? "left" : "right";
}

void BluetoothSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	Q_UNUSED(reading)
}
