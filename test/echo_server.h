#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <memory>
#include "goku/loader.h"
#include "goku/goku.h"

NS_GOKU_BEG

class EchoServer
{
public:
	EchoServer(ILoop *loop);

	~EchoServer();

	void Start();

	void Stop();

private:
	void OnConnection(peer_t peer);

	void OnRead(peer_t peer, void *buf, size_t sz);

	void OnClose(peer_t peer);

private:
	ITcpServer *server_;
};

NS_GOKU_END

#endif // ECHO_SERVER_H
