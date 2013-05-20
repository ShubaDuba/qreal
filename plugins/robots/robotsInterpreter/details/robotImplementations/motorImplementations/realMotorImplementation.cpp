#include "realMotorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

RealMotorImplementation::RealMotorImplementation(int const port, RobotCommunicator *robotCommunicationInterface)
	:  AbstractMotorImplementation(port)
	, mRobotCommunicationInterface(robotCommunicationInterface)
{
}

void RealMotorImplementation::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void RealMotorImplementation::on(int speed, long unsigned int degrees, bool breakMode)
{
	int mode = motorMode::MOTORON | motorMode::REGULATED;
	if (breakMode) {
		mode |= motorMode::BRAKE;
	}
	setOutputState(speed, mode, regulationMode::REGULATION_MODE_MOTOR_SPEED
			, 100, runState::MOTOR_RUN_STATE_RUNNING, degrees);
}

void RealMotorImplementation::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

void RealMotorImplementation::off()
{
	setOutputState(0, motorMode::REGULATED, regulationMode::REGULATION_MODE_MOTOR_SPEED
			, 100, runState::MOTOR_RUN_STATE_IDLE, 0);
}

void RealMotorImplementation::setOutputState(int speed, int mode
		, regulationMode::RegulationModeEnum regulation, int turnRatio
		, runState::RunStateEnum runState, unsigned long tachoLimit)
{
	Q_UNUSED(mode)
	Q_UNUSED(regulation)
	Q_UNUSED(turnRatio)
	Q_UNUSED(runState)
	Q_UNUSED(tachoLimit)

	QString const motorType = mPort == details::outputPort::port3
			? "Right" : "Left";
	QString const command = QString("Motor%1 %2\n").arg(motorType, QString::number(speed));

	mRobotCommunicationInterface->send(this, command.toLatin1(), 0);
}

void RealMotorImplementation::resetMotorPosition(bool relative)
{
	Q_UNUSED(relative)
//	QByteArray command(5, 0);
//	command[0] = 3;  // command length.
//	command[1] = 0x00;
//	command[2] = telegramType::directCommandNoResponse;
//	command[3] = commandCode::RESETMOTORPOSITION;
//	command[4] = relative;
//	mRobotCommunicationInterface->send(this, command, 3);
}
