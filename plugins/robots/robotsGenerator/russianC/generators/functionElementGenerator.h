#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "../russianCRobotGenerator.h"
#include "simpleElementGenerator.h"

namespace robots
{
namespace russianC
{

/// Implementation of AbstractElementGenerator for Function
class FunctionElementGenerator : public SimpleElementGenerator
{
public:
	FunctionElementGenerator(RussianCRobotGenerator *russianC
			, qReal::Id const &elementId
			, bool const &generateToInit);

protected:
	virtual QList<SmartLine> convertBlockIntoCode();
	void variableAnalysis(QByteArray const &code);

private:
	QByteArray replaceSensorVariables(QByteArray portValue) const;
	bool mGenerateToInit;
};

}
}
