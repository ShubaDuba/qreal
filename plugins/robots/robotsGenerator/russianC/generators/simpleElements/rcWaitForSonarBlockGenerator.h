#include "../../../../../../qrkernel/ids.h"
#include "rcAbstractSimpleElementGenerator.h"
#include "../rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

class WaitForSonarBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	WaitForSonarBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	void addInitAndTerminateCode(RussianCRobotGenerator *nxtGen
			, QString const &port,qReal::Id const elementId);
};

}
}
