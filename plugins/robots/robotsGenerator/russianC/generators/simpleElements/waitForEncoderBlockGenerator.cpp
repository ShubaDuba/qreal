#include "waitForEncoderBlockGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

WaitForEncoderBlockGenerator::WaitForEncoderBlockGenerator()
{
}

QList<SmartLine> WaitForEncoderBlockGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = nxtGen->api()->stringProperty(logicElementId, "Port");
	QString const tachoLimit = nxtGen->api()->stringProperty(logicElementId, "TachoLimit");
	result.append(SmartLine("while (nxt_motor_get_count(NXT_PORT_" + port + ") < " + tachoLimit + ")"
			, elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
