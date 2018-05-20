#include <iostream>
#include <assert.h>
#include "echo_server.h"

NS_GOKU_BEG

EchoServer::EchoServer(Loop *loop)
	: server_(loop)
{
	server_.SetOnConnectionCallback(std::bind(&EchoServer::OnConnection, this, std::placeholders::_1));
	server_.SetOnReadCallback(std::bind(&EchoServer::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	server_.SetOnCloseCallback(std::bind(&EchoServer::OnClose, this, std::placeholders::_1));
}


void EchoServer::Start()
{
	int ret = server_.StartListen("0.0.0.0", 55555);
	assert(!ret);
}


void EchoServer::Stop()
{
	server_.StopListen();
}


void EchoServer::OnConnection(peer_t peer)
{
	std::cout << __FUNCTION__ << ": " << peer << std::endl;
}


void EchoServer::OnRead(peer_t peer, void *buf, size_t sz)
{
	std::string cmd((char const*)buf, (char const*)buf + sz);

	server_.Send(peer, buf, sz);
	if (cmd == "quit") {
		server_.Disconnect(peer);
	} else if (cmd == "close_server") {
		Stop();
	}
}


void EchoServer::OnClose(peer_t peer)
{
	std::cout << __FUNCTION__ << ": " << peer << std::endl;
}

NS_GOKU_END
