#include "realBrickImplementation.h"
#include "../../robotCommandConstants.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::brickImplementations;

RealBrickImplementation::RealBrickImplementation(RobotCommunicator * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
{
}

void RealBrickImplementation::playTone(unsigned freq, unsigned time)
{
	QString const pattern = "beep %1 %2\n";
	QString const command = pattern.arg(QString::number(time), QString::number(freq));
	mRobotCommunicationInterface->send(this, command.toLatin1(), 5);
}

void RealBrickImplementation::beep(unsigned time)
{
	playTone(1000, time);
}
