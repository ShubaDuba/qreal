#include "../../../../../../qrkernel/ids.h"
#include "rcAbstractSimpleElementGenerator.h"
#include "../rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

class EnginesGenerator : public AbstractSimpleElementGenerator
{
public:
	EnginesGenerator(QString const &engineType);

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	void addInitAndTerminateCode(RussianCRobotGenerator *nxtGen
			, qReal::Id const elementId, QString const &enginePort);
	QString mEngineType;

};

}
}
