#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

class WaitForTouchSensorBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	WaitForTouchSensorBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
};

}
}
