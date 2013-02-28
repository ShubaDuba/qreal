#include "rcLoopElementGenerator.h"

#include "../russianCRobotGenerator.h"
#include "../rcElementGeneratorFactory.h"

using namespace qReal;
using namespace robots::russianC;

LoopElementGenerator::LoopElementGenerator(RussianCRobotGenerator *emboxGen
		, qReal::Id const &elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

bool LoopElementGenerator::nextElementsGeneration()
{
	IdList const outgoingLinks = mRobotCGenerator->api()->outgoingLinks(mElementId);
	Q_ASSERT(outgoingLinks.size() == 2);

	int elementConnectedByIterationEdgeNumber = -1;
	int afterLoopElementNumber = -1;

	if (mRobotCGenerator->api()->stringProperty(mRobotCGenerator->api()->logicalId(outgoingLinks.at(0)), "Guard").toUtf8() == "итерация") {
		elementConnectedByIterationEdgeNumber = 0;
		afterLoopElementNumber = 1;
	} else {
		elementConnectedByIterationEdgeNumber = 1;
		afterLoopElementNumber = 0;
	}

	//generate loop
	Id const loopNextElement = mRobotCGenerator->api()->to(outgoingLinks.at(elementConnectedByIterationEdgeNumber));
	if (loopNextElement == Id::rootId()) {
		mRobotCGenerator->errorReporter().addError("Loop block " + mElementId.toString() + " has no correct loop branch!"\
				" May be you need to connect it to some diagram element.", mElementId);
		return false;
	}

	AbstractElementGenerator* const loopGen = ElementGeneratorFactory::generator(mRobotCGenerator
			, loopNextElement, *mRobotCGenerator->api());

	mRobotCGenerator->previousElement() = mElementId;
	mRobotCGenerator->previousLoopElements().push(mElementId);
	if (!loopGen->generate()) {
		return false;
	}
	delete loopGen;

	//generate next blocks
	Id const nextBlockElement = mRobotCGenerator->api()->to(outgoingLinks.at(afterLoopElementNumber));
	if (nextBlockElement == Id::rootId()) {
		mRobotCGenerator->errorReporter().addError("Loop block " + mElementId.toString() + " has no correct next block branch!"\
				" May be you need to connect it to some diagram element.", mElementId);
		return false;
	}

	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mRobotCGenerator
			, nextBlockElement, *mRobotCGenerator->api());

	mRobotCGenerator->previousElement() = mElementId;
	mRobotCGenerator->previousLoopElements().push(mElementId);
	if (!nextBlocksGen->generate()) {
		return false;
	}
	delete nextBlocksGen;

	return true;
}

QList<SmartLine> LoopElementGenerator::addLoopCodeInPrefixForm()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mRobotCGenerator->api()->logicalId(mElementId); //TODO
	result << SmartLine(QString::fromUtf8("для целого i от 0 до ")
			+ mRobotCGenerator->api()->property(logicElementId, "Iterations").toString()
				+ ") {", mElementId, SmartLine::increase); //TODO
	return result;
}

QList<SmartLine> LoopElementGenerator::addLoopCodeInPostfixForm()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool LoopElementGenerator::preGenerationCheck()
{
	IdList const outgoingLinks = mRobotCGenerator->api()->outgoingLinks(mElementId);

	if ((outgoingLinks.size() != 2) ||
		((mRobotCGenerator->api()->property(mRobotCGenerator->api()->logicalId(outgoingLinks.at(0)), "Guard").toString() == "итерация")
		&& (mRobotCGenerator->api()->property(mRobotCGenerator->api()->logicalId(outgoingLinks.at(1)), "Guard").toString() == "итерация") )
	) {
		return false;
	}

	return true;
}
