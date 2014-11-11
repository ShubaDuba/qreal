#include "button.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotParts::Button(info, port), mRobotCommunicator(tcpRobotCommunicator)
{
//	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
//			, this, &Button::onIncomingData);
}

void Button::read()
{
	emit newData(1);
//  uncomment when trikRuntime supports button state requests
//	mRobotCommunicator.requestData(port().name());
}

void Button::onIncomingData(QString const &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}
