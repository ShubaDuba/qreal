#include "rcAbstractSimpleElementGenerator.h"

#include "../rcAbstractElementGenerator.h"
#include "../rcSimpleElementFactory.h"

using namespace robots::russianC;

AbstractSimpleElementGenerator::AbstractSimpleElementGenerator()
{
}

AbstractSimpleElementGenerator::~AbstractSimpleElementGenerator()
{
}

QList<SmartLine> AbstractSimpleElementGenerator::convertedCode(RussianCRobotGenerator *nxtGen
		, const qReal::Id elementId, const qReal::Id logicElementId)
{
	QString const elementType = elementId.element();
	AbstractSimpleElementGenerator *currentSimpleElement = SimpleElementFactory::generator(elementType);

	QList<SmartLine> result = currentSimpleElement->convertElementIntoDirectCommand(nxtGen, elementId, logicElementId);
	return result;
}

QList<QString> AbstractSimpleElementGenerator::portsToEngineNames(QString const &portsProperty) const
{
	QList<QString> result;

	if (portsProperty.contains("A")) {
		result.append(QString::fromUtf8("порт_A"));
	}
	if (portsProperty.contains("B")) {
		result.append(QString::fromUtf8("порт_B"));
	}
	if (portsProperty.contains("C")) {
		result.append(QString::fromUtf8("порт_C"));
	}

	return result;
}

QString AbstractSimpleElementGenerator::transformSign(QString const &sign) const
{
	if (sign == "меньше") {
		return "<";
	} else if (sign == "больше"){
		return ">";
	} else if (sign == "не меньше") {
		return ">=";
	} else if (sign == "не больше") {
		return "<=";
	} else if (sign == "равно") {
		return "==";
	}
	return "";
}
