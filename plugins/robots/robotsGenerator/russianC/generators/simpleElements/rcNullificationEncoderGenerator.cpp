#include "rcNullificationEncoderGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

NullificationEncoderGenerator::NullificationEncoderGenerator()
{
}

QList<SmartLine> NullificationEncoderGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString const port = nxtGen->api()->stringProperty(logicElementId, "Port");
	result.append(SmartLine(QString::fromUtf8("установить_энкодер(порт_") + port + ", 0);", elementId));

	return result;
}
