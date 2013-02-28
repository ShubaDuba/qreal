#include "rcWaitForLightBlockGenerator.h"
#include "../../russianCRobotGenerator.h"
#include "../rcListsmartline.h"

using namespace robots::russianC;

WaitForLightBlockGenerator::WaitForLightBlockGenerator()
{
}

void WaitForLightBlockGenerator::addInitAndTerminateCode(RussianCRobotGenerator *nxtGen
			, QString const &port, qReal::Id const elementId)
{
	QString const initCode = QString::fromUtf8("подготовить_сенсор_света(") + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = QString::fromUtf8("остановить_сенсор_света(") + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForLightBlockGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = QString::fromUtf8("порт_") + nxtGen->api()->stringProperty(logicElementId, "Port");

	QString const percents = nxtGen->api()->stringProperty(logicElementId, "Percents");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + percents;

	result.append(SmartLine(QString::fromUtf8("пока (не (сенсор_света(") + port
			+ ") " + condition + ")) {", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);
	return result;
}
