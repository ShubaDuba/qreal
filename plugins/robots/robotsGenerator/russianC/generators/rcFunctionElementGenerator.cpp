#include "rcFunctionElementGenerator.h"

using namespace robots::russianC;

FunctionElementGenerator::FunctionElementGenerator(RussianCRobotGenerator *generator
		, qReal::Id const &elementId, bool const &generateToInit)
		: SimpleElementGenerator(generator, elementId), mGenerateToInit(generateToInit)
{
}

void FunctionElementGenerator::variableAnalysis(QByteArray const &code)
{
	QList<QByteArray> const funcBlocks = code.split(';');

	foreach (QByteArray const &block, funcBlocks) {
			//Only one possible place for first variable appear
		int const firstEqualSignPos = block.indexOf('=');
		if (firstEqualSignPos == -1) {
			continue;
		}

		//must be a normal variable name
		QByteArray leftPart = block.left(firstEqualSignPos);

		leftPart = leftPart.trimmed();
		QString const forbiddenLastSimbols = "+-=*/><";
		if (forbiddenLastSimbols.contains((leftPart.at(leftPart.length() - 1)))) {
			continue;
		}

		bool isVariableExisted = false;
		foreach (SmartLine const &curVariable, mRobotCGenerator->variables()) {
			if (curVariable.text() == QString::fromUtf8(leftPart)) {
				isVariableExisted = true;
				break;
			}
		}
		if (!isVariableExisted) {
			mRobotCGenerator->variables().append(SmartLine(QString::fromUtf8(leftPart), mElementId));
		}
	}
}

QByteArray FunctionElementGenerator::replaceSensorVariables(QByteArray portValue) const
{
	if ((portValue == "Сенсор цвета (красный)")
			|| (portValue == "Сенсор цвета (зеленый)")
			|| (portValue == "Сенсор цвета (синий)")
			|| (portValue == "Сенсор цвета (все цвета)")
			|| (portValue == "Сенсор цвета (пассивный)"))
	{
		return "сенсор_цвета(порт_";
	} else if (portValue == "Ультразвуковой сенсор") {
		return "сенсор_расстояния(порт_";
	} else {
		return "сенсор_касания(порт_";
	}
}

QList<SmartLine> FunctionElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mRobotCGenerator->api()->logicalId(mElementId); //TODO

	QByteArray byteFuncCode = mRobotCGenerator->api()->stringProperty(logicElementId, "Body").toUtf8();

	byteFuncCode.replace("Sensor1", replaceSensorVariables(mRobotCGenerator->portValue1()) + "1)");
	byteFuncCode.replace("Sensor2", replaceSensorVariables(mRobotCGenerator->portValue2()) + "2)");
	byteFuncCode.replace("Sensor3", replaceSensorVariables(mRobotCGenerator->portValue3()) + "3)");
	byteFuncCode.replace("Sensor4", replaceSensorVariables(mRobotCGenerator->portValue4()) + "4)");

	variableAnalysis(byteFuncCode);
	QString const funcCode = QString::fromUtf8(byteFuncCode);

	foreach (QString const &str, funcCode.split(';')) {
		result.append(SmartLine(str.trimmed() + ";", mElementId));
	}

	if (mGenerateToInit) {
		mRobotCGenerator->initCode().append(result);
		result.clear();
	}

	return result;
}
