#pragma once

#include <QtNetwork/QTcpSocket>

#include "robotCommunicationThreadInterface.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{

class TcpCommunicationThread : public RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	TcpCommunicationThread();
	virtual ~TcpCommunicationThread();

public slots:
	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer
			, unsigned const responseSize, inputPort::InputPortEnum const &port);
	virtual void connect();
	virtual void disconnect();
	virtual void reconnect();
	virtual void allowLongJobs(bool allow = true);
	virtual void checkConsistency();

private:
	QTcpSocket mSocket;
};

}
}
}
}
