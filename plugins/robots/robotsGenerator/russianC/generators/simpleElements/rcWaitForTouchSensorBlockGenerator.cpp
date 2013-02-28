#include "rcWaitForTouchSensorBlockGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

WaitForTouchSensorBlockGenerator::WaitForTouchSensorBlockGenerator()
{
}

QList<SmartLine> WaitForTouchSensorBlockGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();

	result.append(SmartLine(QString::fromUtf8("пока (не сенсор_касания(порт_") + QString::number(port) + ")) {"
			, elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
