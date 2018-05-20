#include <assert.h>
#include <memory>
#include "goku/tcp_server.h"
#include "tcp_connection.h"
#include "tcp_server_impl.h"

NS_GOKU_BEG

TcpServer::TcpServer(Loop *loop)
	: impl_(new TcpServerImpl(loop))
{
}


TcpServer::~TcpServer()
{
	delete impl_;
}


int TcpServer::StartListen(char const *ip, int port)
{
	return impl_->StartListen(ip, port);
}


int TcpServer::StopListen()
{
	return impl_->StopListen();
}


void TcpServer::SetOnConnectionCallback(on_connection_cb_t const cb)
{
	impl_->SetOnConnectionCallback(cb);
}


void TcpServer::SetOnReadCallback(on_read_cb_t const &cb)
{
	impl_->SetOnReadCallback(cb);
}


void TcpServer::SetOnCloseCallback(on_close_cb_t const &cb)
{
	impl_->SetOnShutdownCallback(cb);
}


int TcpServer::Send(uint64_t peer, void *data, size_t sz)
{
	return impl_->Send(peer, data, sz);
}


int TcpServer::Disconnect(uint64_t peer)
{
	return impl_->Disconnect(peer);
}

NS_GOKU_END
