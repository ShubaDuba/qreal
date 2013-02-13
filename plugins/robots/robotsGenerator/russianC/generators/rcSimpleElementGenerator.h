#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../rcSmartLine.h"
#include "rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

/// Implementation of AbstractElementGenerator for Beep, Engines etc
class SimpleElementGenerator : public AbstractElementGenerator
{
public:
	explicit SimpleElementGenerator(RussianCRobotGenerator *mainGenerator, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm();
	virtual QList<SmartLine> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	virtual QList<SmartLine> convertBlockIntoCode();
};

}
}
