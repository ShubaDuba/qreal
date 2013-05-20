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
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandResponseRequired;
	command[3] = commandCode::GETOUTPUTSTATE;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 27);
}

	// not implemented
}

void BluetoothEncoderImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	Q_UNUSED(reading)
}
