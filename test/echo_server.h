#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <memory>
#include "goku/tcp_server.h"

NS_GOKU_BEG

class EchoServer
{
public:
	EchoServer(Loop *loop);

	void Start();

	void Stop();

private:
	void OnConnection(peer_t peer);

	void OnRead(peer_t peer, void *buf, size_t sz);

	void OnClose(peer_t peer);

private:
	TcpServer server_;
};

NS_GOKU_END

#endif // ECHO_SERVER_H
