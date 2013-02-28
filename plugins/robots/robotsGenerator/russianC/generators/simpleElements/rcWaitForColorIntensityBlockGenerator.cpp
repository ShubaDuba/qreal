#include "rcWaitForColorIntensityBlockGenerator.h"
#include "../../russianCRobotGenerator.h"
#include "../rcListsmartline.h"

using namespace robots::russianC;

WaitForColorIntensityBlockGenerator::WaitForColorIntensityBlockGenerator()
{
}

void WaitForColorIntensityBlockGenerator::addInitAndTerminateCode(RussianCRobotGenerator *nxtGen
		, QString const &port, qReal::Id const elementId)
{
	QString const partInitCode = QString::fromUtf8("подготовить_сенсор_цвета(") + port;
	if (!ListSmartLine::isContainsPart(nxtGen->initCode(), partInitCode)) {
		// i don't no, mb NXT_LIGHTSENSOR_NONE, NXT_COLORSENSOR_DEACTIVATE
		QString const initCode = QString::fromUtf8("подготовить_сенсор_цвета(") + port + ");";
		QString const terminateCode = QString::fromUtf8("остановить_сенсор_цвета(") + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForColorIntensityBlockGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString portStr = nxtGen->api()->stringProperty(logicElementId, "Port");
	QString const intensity = nxtGen->api()->stringProperty(logicElementId,  "Intensity");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + intensity;

	result.append(SmartLine(QString::fromUtf8("пока (не (сенсор_света(порт_") + portStr
			+ ") " + condition + ")) {", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, QString::fromUtf8("порт_") + portStr, elementId);

	return result;
}
