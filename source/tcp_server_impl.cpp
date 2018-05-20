#include <assert.h>
#include "tcp_server_impl.h"
#include "tcp_connection.h"

NS_GOKU_BEG

TcpServerImpl::TcpServerImpl(Loop *loop)
	: loop_(loop)
{
}


TcpServerImpl::~TcpServerImpl()
{
}


int TcpServerImpl::StartListen(char const *ip, int port)
{
	struct sockaddr_in addr;
	int ret = uv_ip4_addr(ip, port, &addr);
	if (ret) { return -1; }

	ret = uv_tcp_init(*loop_, &server_);
	if (ret) { return -1; }
	server_.data = this;

	ret = uv_tcp_bind(&server_, (const struct sockaddr*) &addr, 0);
	if (ret) { return -1; }

	ret = uv_listen((uv_stream_t*)&server_, SOMAXCONN, &TcpServerImpl::S_OnConnection);
	if (ret) { return -1; }

	return 0;
}


int TcpServerImpl::StopListen()
{
	uv_close((uv_handle_t*)&server_, &TcpServerImpl::S_OnClose);
	return 0;
}


void TcpServerImpl::SetOnConnectionCallback(on_connection_cb_t const cb)
{
	on_connection_ = cb;
}


void TcpServerImpl::SetOnReadCallback(on_read_cb_t const &read_cb)
{
	on_read_ = read_cb;
}


void TcpServerImpl::SetOnShutdownCallback(on_close_cb_t const &cb)
{
	on_close_ = cb;
}


int TcpServerImpl::Send(uint64_t peer, void *data, size_t sz)
{
	auto iter = clients_.find(peer);
	if (iter == clients_.end()) { return -1; }
	return iter->second->Send(data, sz);
}


int TcpServerImpl::Disconnect(uint64_t peer)
{
	auto iter = clients_.find(peer);
	if (iter == clients_.end()) { return -1; }
	return iter->second->Close();
}


void TcpServerImpl::S_OnConnection(uv_stream_t* server, int status)
{
	TcpServerImpl *self = (TcpServerImpl*)server->data;
	self->OnConnection(server, status);
}


void TcpServerImpl::OnConnection(uv_stream_t* server, int status)
{
	assert(status == 0);

	std::unique_ptr<uv_tcp_t> stream(new uv_tcp_t());
	int ret = uv_tcp_init(*loop_, &*stream);
	assert(ret == 0);

	stream->data = this;

	ret = uv_accept(server, (uv_stream_t*)&*stream);
	assert(ret == 0);

	TcpConnection *connection = new TcpConnection(this, std::move(stream));
	clients_.insert(std::make_pair(uint64_t(connection), std::unique_ptr<TcpConnection>(connection)));
	
	on_connection_(uint64_t(connection));

	connection->StartRead();
}


void TcpServerImpl::S_OnClose(uv_handle_t* handle)
{
	// TODO:
}


void TcpServerImpl::OnClose(TcpConnection *connection)
{
	on_close_(uint64_t(connection));
	auto iter = clients_.find(uint64_t(connection));
	assert(iter != clients_.end());
	clients_.erase(iter);
}


void TcpServerImpl::OnRead(TcpConnection *connection, char const *buf, size_t sz)
{
	on_read_(uint64_t(connection), (void*)buf, sz);
}

NS_GOKU_END
