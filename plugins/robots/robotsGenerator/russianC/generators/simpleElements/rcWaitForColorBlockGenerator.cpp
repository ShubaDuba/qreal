#include "rcWaitForColorBlockGenerator.h"
#include "../../russianCRobotGenerator.h"
#include "../rcListsmartline.h"

using namespace robots::russianC;

WaitForColorBlockGenerator::WaitForColorBlockGenerator()
{
}

void WaitForColorBlockGenerator::addInitAndTerminateCode(RussianCRobotGenerator *nxtGen,
		QString const &port, QString const &colorNxtType, qReal::Id const elementId)
{
	QString const partInitCode = QString::fromUtf8("подготовить_сенсор_цвета(") + port;
	QString const initCode = QString::fromUtf8("подготовить_сенсор_цвета(") + port + "," + colorNxtType + ");";

	if (!ListSmartLine::isContainsPart(nxtGen->initCode(), partInitCode)) {
		// There was was init_.... earlier. Bug? Then fix it in nxtOSEK generator
		QString const terminateCode = QString::fromUtf8("остановить_сенсор_цвета(") + port + "," + colorNxtType + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForColorBlockGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();
	QByteArray const colorStr = nxtGen->api()->stringProperty(logicElementId, "Color").toUtf8();

	QString colorNxtType = colorStr;

	if (!colorNxtType.isEmpty()) {
		QString portStr = QString::number(port);
		result.append(SmartLine(QString::fromUtf8("пока (сенсора_цвета(порт_") + portStr
				+ ") != " + colorNxtType + ")", elementId));
		result.append(SmartLine("{", elementId));
		result.append(SmartLine("}", elementId));
		addInitAndTerminateCode(nxtGen, portStr, colorNxtType, elementId);
	}

	return result;
}
