#include "rcAbstractElementGenerator.h"
#include "../russianCRobotGenerator.h"
#include "../rcElementGeneratorFactory.h"

using namespace robots::russianC;

AbstractElementGenerator::AbstractElementGenerator(RussianCRobotGenerator *mainGenerator
		, qReal::Id const &elementId)
	: mRobotCGenerator(mainGenerator), mElementId(elementId)
{
}

AbstractElementGenerator::~AbstractElementGenerator()
{
}

void AbstractElementGenerator::createListsForIncomingConnections()
{
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mRobotCGenerator->api()->incomingConnectedElements(mElementId).size(); i++) {
		mRobotCGenerator->generatedStringSet() << QList<SmartLine>();
		mRobotCGenerator->elementToStringListNumbers()[mElementId.toString()] << mRobotCGenerator->generatedStringSet().size() - 1;
	}
}

bool AbstractElementGenerator::generate()
{
	if (!preGenerationCheck()) {
		return false;
	}

	if (mRobotCGenerator->elementToStringListNumbers().contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection

		qReal::Id loopElement = mElementId;
		if (!mRobotCGenerator->previousLoopElements().empty()) {
			loopElement = mRobotCGenerator->previousLoopElementsPop();
		}

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mRobotCGenerator, loopElement, *mRobotCGenerator->api());

		int num = mRobotCGenerator->elementToStringListNumbersPop(loopElement.toString());
		QList<SmartLine> set = mRobotCGenerator->generatedStringSet()[num] + loopElementGen->addLoopCodeInPrefixForm();
		mRobotCGenerator->setGeneratedStringSet(num, set);
		mRobotCGenerator->generatedStringSet() << loopElementGen->addLoopCodeInPostfixForm();

		return true;
	}

	//in case element has more than 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	return nextElementsGeneration();
}
