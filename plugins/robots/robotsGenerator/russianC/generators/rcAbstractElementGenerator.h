#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../rcSmartLine.h"

namespace robots
{
namespace russianC
{

class RussianCRobotGenerator;

/// AbstractElementGenerator - robot diagram element generator abstraction.
class AbstractElementGenerator
{
public:
	AbstractElementGenerator(RussianCRobotGenerator *mainGenerator, qReal::Id const &elementId);
	virtual ~AbstractElementGenerator();
	virtual bool generate();

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm() = 0;
	virtual QList<SmartLine> addLoopCodeInPostfixForm() = 0;

	/**
	 * Creates new lists in mGeneratedStringSet
	 * and connects it with mElementId in mElementToStringListNumbers
	 * in case element have more than 1 incoming connection.
	*/
	virtual void createListsForIncomingConnections();
	virtual bool preGenerationCheck() = 0;
	virtual bool nextElementsGeneration() = 0;

	RussianCRobotGenerator *mNxtGen;
	qReal::Id mElementId;
};

}
}
