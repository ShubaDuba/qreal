#include "rcWaitForSonarBlockGenerator.h"
#include "../../russianCRobotGenerator.h"
#include "../rcListsmartline.h"

using namespace robots::russianC;

WaitForSonarBlockGenerator::WaitForSonarBlockGenerator()
{
}

void WaitForSonarBlockGenerator::addInitAndTerminateCode(RussianCRobotGenerator *nxtGen
			, QString const &port, qReal::Id const elementId)
{
	QString const initCode = "ecrobot_init_sonar_sensor(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_term_sonar_sensor(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}
QList<SmartLine> WaitForSonarBlockGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");
	QString const distance = nxtGen->api()->stringProperty(logicElementId, "Distance");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));
	QString const condition = inequalitySign + " " + distance;

	result.append(SmartLine("while (!(ecrobot_get_sonar_sensor(" + port + ")"
			+ condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);

	return result;
}
