#pragma once

#include "../../../../qrkernel/ids.h"
#include "russianCRobotGenerator.h"
#include "generators/rcAbstractElementGenerator.h"

namespace robots
{
namespace russianC
{

/// Element generator factory that returns generator for diagram element Id.
class ElementGeneratorFactory
{
public:
	static AbstractElementGenerator* generator(
			RussianCRobotGenerator *generator
			, qReal::Id const &elementId
			, qrRepo::RepoApi const &api
		);
};

}
}
