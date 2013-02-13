#include "../../../../../../qrkernel/ids.h"
#include "rcAbstractSimpleElementGenerator.h"
#include "../rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

class WaitForColorBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	WaitForColorBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	void addInitAndTerminateCode(RussianCRobotGenerator *nxtGen,
			QString const &port, QString const &colorNxtType, qReal::Id const elementId);
};

}
}
