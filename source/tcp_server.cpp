#include <assert.h>
#include "tcp_server.h"
#include "tcp_connection.h"

NS_GOKU_BEG

TcpServer::TcpServer(Loop *loop)
	: loop_(loop)
{
}


TcpServer::~TcpServer()
{
}


int TcpServer::StartListen(char const *ip, int port)
{
	struct sockaddr_in addr;
	int ret = uv_ip4_addr(ip, port, &addr);
	if (ret) { return -1; }

	server_.reset(new uv_tcp_t());
	ret = uv_tcp_init(*loop_, &*server_);
	if (ret) { return -1; }
	server_->data = this;

	ret = uv_tcp_bind(&*server_, (const struct sockaddr*) &addr, 0);
	if (ret) { return -1; }

	ret = uv_listen((uv_stream_t*)&*server_, SOMAXCONN, &TcpServer::S_OnConnection);
	if (ret) { return -1; }

	return 0;
}


int TcpServer::StopListen()
{
	if (!server_) { return -1; }
	if (uv_is_closing((uv_handle_t*)&*server_)) { return -1; }
	uv_close((uv_handle_t*)&*server_, &TcpServer::S_OnClose);
	return 0;
}


void TcpServer::SetOnConnectionCallback(on_connection_cb_t const cb)
{
	on_connection_ = cb;
}


void TcpServer::SetOnReadCallback(on_read_cb_t const &read_cb)
{
	on_read_ = read_cb;
}


void TcpServer::SetOnCloseCallback(on_close_cb_t const &cb)
{
	on_close_ = cb;
}


int TcpServer::Send(peer_t peer, void *data, size_t sz)
{
	auto iter = clients_.find(peer);
	if (iter == clients_.end()) { return -1; }
	return iter->second->Send(data, sz);
}


int TcpServer::Disconnect(peer_t peer)
{
	auto iter = clients_.find(peer);
	if (iter == clients_.end()) { return -1; }
	return iter->second->Close();
}


void TcpServer::S_OnConnection(uv_stream_t* server, int status)
{
	TcpServer *self = (TcpServer*)server->data;
	self->OnConnection(server, status);
}


void TcpServer::OnConnection(uv_stream_t* server, int status)
{
	assert(status == 0);

	std::unique_ptr<uv_tcp_t> stream(new uv_tcp_t());
	int ret = uv_tcp_init(*loop_, &*stream);
	assert(ret == 0);

	stream->data = this;

	ret = uv_accept(server, (uv_stream_t*)&*stream);
	assert(ret == 0);

	TcpConnection *connection = new TcpConnection(std::move(stream), 
		std::bind(&TcpServer::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&TcpServer::OnClose, this, std::placeholders::_1));
	clients_.insert(std::make_pair(peer_t(connection), std::unique_ptr<TcpConnection>(connection)));
	
	on_connection_(peer_t(connection));

	connection->Init();
}


void TcpServer::S_OnClose(uv_handle_t* handle)
{
	// TODO:
	TcpServer *self = (TcpServer*)handle->data;
	assert(self);
	self->server_.reset();
}


void TcpServer::OnClose(TcpConnection *connection)
{
	on_close_(peer_t(connection));
	auto iter = clients_.find(peer_t(connection));
	assert(iter != clients_.end());
	clients_.erase(iter);
}


void TcpServer::OnRead(TcpConnection *connection, void *buf, size_t sz)
{
	on_read_(peer_t(connection), (void*)buf, sz);
}

NS_GOKU_END
