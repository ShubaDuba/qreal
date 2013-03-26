#include "bluetoothEncoderImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothEncoderImplementation::BluetoothEncoderImplementation(RobotCommunicator *robotCommunicationInterface
		, outputPort::OutputPortEnum const &port)
	: AbstractEncoderImplementation(port)
{
	mRobotCommunicationInterface = robotCommunicationInterface;
//	connect(mRobotCommunicationInterface, SIGNAL(response(QObject*,QByteArray))
//			, this, SLOT(readingDone(QObject*,QByteArray)));
}

void BluetoothEncoderImplementation::read()
{
	if (mState == pending) {
		return;
	}
	mState = pending;

	// not implemented
}

void BluetoothEncoderImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	Q_UNUSED(reading)
}
