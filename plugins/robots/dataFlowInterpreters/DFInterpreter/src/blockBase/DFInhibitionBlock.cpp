#include "DFInhibitionBlock.h"

using namespace dataFlowBlocks::details;


DFInhibitionBlock::DFInhibitionBlock()
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["STOP"] = 0;
		portAssociatedWithProperty["DATA"] = 1;
		portAssociatedWithProperty["OUT"] = 2;
		portAssociatedWithProperty["TIME"] = 3;

		int time = intProperty("time");
		if (time) {
			timeToInhibit = time;
		}
}

void DFInhibitionBlock::handleData()
{
	if (!inhibitionStarted) {
		if (hasNewProperty("STOP")) {
			inhibitionStarted = true;
			startInhibitionTime = QTime::currentTime();
			flushData();
			return;
		}

		if (hasNewProperty("TIME")) {
			timeToInhibit = propertyFromPort("TIME").toInt();
		}

		if (hasNewProperty("DATA")) {
			emit newDataInFlow(propertyFromPort("DATA"), portAssociatedWithProperty["OUT"]);
		}
	} else {
		if (QTime::currentTime().msecsTo(startInhibitionTime) >= timeToInhibit) {
			inhibitionStarted = false;
			handleData();
		} else {
			flushData();
		}
	}
}

void DFInhibitionBlock::flushData()
{
	propertyFromPort("DATA"); // flush data
	propertyFromPort("STOP"); // flush data
	propertyFromPort("TIME"); // flush data
}
