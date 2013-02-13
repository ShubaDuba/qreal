#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "../rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

class AbstractSimpleElementGenerator
{
public:
	AbstractSimpleElementGenerator();
	virtual ~AbstractSimpleElementGenerator();

	static QList<SmartLine> convertedCode(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId) = 0;
	virtual QList<QString> portsToEngineNames(QString const &portsProperty) const;
	virtual QString transformSign(QString const &sign) const;
};

}
}
