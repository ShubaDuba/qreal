#include "rcEnginesGenerator.h"
#include "../../russianCRobotGenerator.h"
#include "../rcListsmartline.h"

using namespace robots::russianC;

EnginesGenerator::EnginesGenerator(QString const &engineType): mEngineType(engineType)
{
}

void EnginesGenerator::addInitAndTerminateCode(RussianCRobotGenerator *nxtGen  // duplicated code - not true
		, qReal::Id const elementId, QString const &enginePort)
{
	QString const initTerminateVelocity = "0";
	QString const initTerminateBrakeMode = "1";
	QString const initCodeEngines = QString::fromUtf8("задать_скорость_мотора(")
			+ enginePort + ", "
			+ initTerminateVelocity + ", "
			+ initTerminateBrakeMode + ");";
	QString const terminateCodeEngines = QString::fromUtf8("задать_скорость_мотора(")
			+ enginePort + ", "
			+ initTerminateVelocity + ", "
			+ initTerminateBrakeMode + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCodeEngines)) {
		nxtGen->initCode().append(SmartLine(initCodeEngines, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCodeEngines, elementId));
	}
}

QList<SmartLine> EnginesGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		 , qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString const power = nxtGen->api()->stringProperty(logicElementId, "Power");
	QByteArray brakeMode = "1";
	QString signRotate = "";
	if (mEngineType == "EnginesBackward") {
		signRotate = "-";
	}

	foreach (QString const &enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {

		result.append(SmartLine(QString::fromUtf8("задать_скорость_мотора(")
				+ enginePort + ", "
				+ signRotate + power + ", "
				+ brakeMode + ");", elementId));
		addInitAndTerminateCode(nxtGen, elementId, "" + enginePort);
	}

	return result;
}
