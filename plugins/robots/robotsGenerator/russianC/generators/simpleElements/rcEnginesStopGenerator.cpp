#include "rcEnginesStopGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

EnginesStopGenerator::EnginesStopGenerator()
{
}

QList<SmartLine> EnginesStopGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	foreach (QString enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(SmartLine("nxt_motor_set_speed(" + enginePort + ", 0, 1);", elementId));
	}

	return result;
}
