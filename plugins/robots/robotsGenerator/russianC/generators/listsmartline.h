#pragma once

#include <QtCore/QList>
#include "../smartLine.h"

namespace robots
{
namespace russianC
{

class ListSmartLine
{
public:
	ListSmartLine();
	static bool isContains(QList<SmartLine> const &storage, QString const &templateCode);
	static bool isContainsPart(QList<SmartLine> const &storage, QString const &templateCode); // rename
};

}
}
