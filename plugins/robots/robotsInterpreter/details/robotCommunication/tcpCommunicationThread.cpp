#include <QtNetwork/QHostAddress>

#include "tcpCommunicationThread.h"
#include "../../../../../qrkernel/settingsManager.h"

using namespace qReal::interpreters::robots::details;

TcpCommunicationThread::TcpCommunicationThread()
{
}

TcpCommunicationThread::~TcpCommunicationThread()
{
}

void TcpCommunicationThread::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	mSocket.write(buffer);
	if (responseSize > 0) {
		mSocket.waitForReadyRead();
		emit response(addressee, mSocket.read(responseSize));
	} else {
		emit response(addressee, QByteArray());
	}
}

void TcpCommunicationThread::sendI2C(QObject *addressee, QByteArray const &buffer
		, unsigned const responseSize, inputPort::InputPortEnum const &port)
{
	Q_UNUSED(addressee)
	Q_UNUSED(buffer)
	Q_UNUSED(responseSize)
	Q_UNUSED(port)
}

void TcpCommunicationThread::connect()
{
	QString const server = SettingsManager::value("tcpServer").toString();
	uint const port = SettingsManager::value("tcpPort").toUInt();
	QHostAddress hostAddress(server);
	if (hostAddress.isNull()) {
		QString const message = tr("Unable to resolve %1. Check server address anfd try again");
		emit errorOccured(message.arg(server));
	}
	mSocket.connectToHost(hostAddress, static_cast<quint16>(port));
}

void TcpCommunicationThread::disconnect()
{
	mSocket.disconnectFromHost();
}

void TcpCommunicationThread::reconnect()
{
	connect();
}

void TcpCommunicationThread::allowLongJobs(bool allow)
{
	Q_UNUSED(allow)
}

void TcpCommunicationThread::checkConsistency()
{
}
