#include "../../../../../../qrkernel/ids.h"
#include "rcAbstractSimpleElementGenerator.h"
#include "../rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

class VariableInitGenerator : public AbstractSimpleElementGenerator
{
public:
	VariableInitGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
};

}
}
