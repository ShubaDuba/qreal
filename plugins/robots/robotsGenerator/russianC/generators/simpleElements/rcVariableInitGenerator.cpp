#include "rcVariableInitGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

VariableInitGenerator::VariableInitGenerator()
{
}

QList<SmartLine> VariableInitGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const variableName = nxtGen->api()->stringProperty(logicElementId, "variable");
	QString const variableValue = nxtGen->api()->stringProperty(logicElementId, "value");
	result.append(SmartLine(variableName + " = " + variableValue + "; \n", elementId));

	return result;
}
