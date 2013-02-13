#include "rcElementGeneratorFactory.h"

#include "generators/rcIfElementGenerator.h"
#include "generators/rcLoopElementGenerator.h"
#include "generators/rcFunctionElementGenerator.h"
#include "generators/rcSimpleElementGenerator.h"

using namespace robots::russianC;

AbstractElementGenerator* ElementGeneratorFactory::generator(
		RussianCRobotGenerator *generator
		, qReal::Id const &elementId
		, qrRepo::RepoApi const &api)
{
	if (elementId.element() == "IfBlock") {
		return new IfElementGenerator(generator, elementId);
	} else if (elementId.element() == "Loop") {
		return new LoopElementGenerator(generator, elementId);
	} else if (elementId.element() == "Function") {
		qReal::Id const logicElementId = api.logicalId(elementId);
		return new FunctionElementGenerator(generator, elementId, api.property(logicElementId, "Init").toBool());
	}

	return new SimpleElementGenerator(generator, elementId);
}
