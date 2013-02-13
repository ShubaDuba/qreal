#include "rcListsmartline.h"

using namespace robots::russianC;

ListSmartLine::ListSmartLine()
{
}

bool ListSmartLine::isContains(QList<SmartLine> const &storage, QString const &templateCode)
{
	foreach (SmartLine const &code, storage) {
		if (code.text() == templateCode) {
			return true;
		}
	}
	return false;
}

bool ListSmartLine::isContainsPart(QList<SmartLine> const &storage, QString const &templateCode)
{
	foreach (SmartLine const &code, storage) {
		if (templateCode.compare(code.text()) <= 0) {
			return true;
		}
	}
	return false;
}
