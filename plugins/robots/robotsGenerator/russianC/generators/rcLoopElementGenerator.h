#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../rcSmartLine.h"
#include "rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

/// Implementation of AbstractElementGenerator for Loop
class LoopElementGenerator : public AbstractElementGenerator
{
public:
	explicit LoopElementGenerator(RussianCRobotGenerator *emboxGen, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm();
	virtual QList<SmartLine> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();
};

}
}
