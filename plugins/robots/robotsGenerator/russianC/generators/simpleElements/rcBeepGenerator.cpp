#include "rcBeepGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

BeepGenerator::BeepGenerator()
{
}

QList<SmartLine> BeepGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine(QString::fromUtf8("играть_звук(1000, 100, ") + nxtGen->api()->stringProperty(logicElementId
			, "Volume") + ");", elementId));

	return result;
}
