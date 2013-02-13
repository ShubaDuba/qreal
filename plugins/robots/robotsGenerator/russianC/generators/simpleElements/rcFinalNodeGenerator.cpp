#include "rcFinalNodeGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

FinalNodeGenerator::FinalNodeGenerator()
{
}

QList<SmartLine> FinalNodeGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(nxtGen)
	Q_UNUSED(logicElementId)

	QList<SmartLine> result;
	result.append(SmartLine("TerminateTask();", elementId));

	return result;
}
