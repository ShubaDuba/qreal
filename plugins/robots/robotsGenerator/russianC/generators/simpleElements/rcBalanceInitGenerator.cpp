#include "rcBalanceInitGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

BalanceInitGenerator::BalanceInitGenerator()
{
}

QList<SmartLine> BalanceInitGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;

	result.append(SmartLine("balance_init();", elementId));
	nxtGen->mBalancerIsActivated = true;

	return result;
}
