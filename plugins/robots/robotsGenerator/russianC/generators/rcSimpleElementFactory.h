#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../russianCRobotGenerator.h"
#include "simpleElements/rcAbstractSimpleElementGenerator.h"

namespace robots
{
namespace russianC
{

/// Element generator factory that returns generator for diagram element Id.
class SimpleElementFactory
{
public:
	static AbstractSimpleElementGenerator* generator(QString const &elementType);
};

}
}
