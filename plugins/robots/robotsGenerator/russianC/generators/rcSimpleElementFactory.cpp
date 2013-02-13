#include "rcSimpleElementFactory.h"
#include "simpleElements/rcEnginesGenerator.h"
#include "simpleElements/rcEnginesStopGenerator.h"
#include "simpleElements/rcTimerGenerator.h"
#include "simpleElements/rcBeepGenerator.h"
#include "simpleElements/rcPlayToneGenerator.h"
#include "simpleElements/rcFinalNodeGenerator.h"
#include "simpleElements/rcNullificationEncoderGenerator.h"
#include "simpleElements/rcInitialBlockGenerator.h"
#include "simpleElements/rcWaitForColorBlockGenerator.h"
#include "simpleElements/rcWaitForColorIntensityBlockGenerator.h"
#include "simpleElements/rcBalanceGenerator.h"
#include "simpleElements/rcWaitForLightBlockGenerator.h"
#include "simpleElements/rcWaitForSonarBlockGenerator.h"
#include "simpleElements/rcWaitForEncoderBlockGenerator.h"
#include "simpleElements/rcWaitForTouchSensorBlockGenerator.h"
#include "simpleElements/rcVariableInitGenerator.h"
#include "simpleElements/rcBalanceInitGenerator.h"
#include "simpleElements/rcInitialNodeGenerator.h"

using namespace robots::russianC;

AbstractSimpleElementGenerator* SimpleElementFactory::generator(QString const &elementType)
{
	if (elementType == "EnginesForward" || elementType == "EnginesBackward") {
		return new EnginesGenerator(elementType);
	} else if (elementType == "EnginesStop") {
		return new EnginesStopGenerator();
	} else if (elementType == "Timer") {
		return new TimerGenerator();
	} else if (elementType == "Beep") {
		return new BeepGenerator();
	} else if (elementType == "PlayTone") {
		return new PlayToneGenerator();
	} else if (elementType == "FinalNode") {
		return new FinalNodeGenerator();
	} else if (elementType == "NullificationEncoder") {
		return new NullificationEncoderGenerator();
	} else if (elementType == "InitialBlock") {
		return new InitialBlockGenerator();
	} else if (elementType == "WaitForColor") {
		return new WaitForColorBlockGenerator();
	} else if (elementType == "WaitForColorIntensity") {
		return new WaitForColorIntensityBlockGenerator();
	} else if (elementType == "WaitForLight") {
		return new WaitForLightBlockGenerator();
	} else if (elementType == "WaitForTouchSensor") {
		return new WaitForTouchSensorBlockGenerator();
	} else if (elementType == "WaitForSonarDistance") {
		return new WaitForSonarBlockGenerator();
	} else if (elementType == "WaitForEncoder") {
		return new WaitForEncoderBlockGenerator();
	} else if (elementType == "Balance") {
		return new BalanceGenerator();
	} else if (elementType == "BalanceInit") {
		return new BalanceInitGenerator();
	} else if (elementType == "VariableInit") {
		return new VariableInitGenerator();
	}

	return new InitialNodeGenerator();
}
