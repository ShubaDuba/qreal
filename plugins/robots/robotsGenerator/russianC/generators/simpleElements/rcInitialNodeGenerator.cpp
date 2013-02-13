#include "rcInitialNodeGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

InitialNodeGenerator::InitialNodeGenerator()
{
}

QList<SmartLine> InitialNodeGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED (nxtGen)
	Q_UNUSED (elementId)
	Q_UNUSED (logicElementId)

	QList<SmartLine> result;

	return result;
}
