#include "rcTimerGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

TimerGenerator::TimerGenerator()
{
}

QList<SmartLine> TimerGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine("systick_wait_ms(" + nxtGen->api()->stringProperty(logicElementId
			, "Delay") + ");", elementId));

	return result;
}
