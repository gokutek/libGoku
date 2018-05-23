#include <assert.h>
#include "tcp_client.h"
#include "tcp_connection.h"

NS_GOKU_BEG

TcpClient::TcpClient(Loop *loop)
	: loop_(loop)
{
}


TcpClient::~TcpClient()
{
}


int TcpClient::Connect(char const *ip, int port)
{
	struct sockaddr_in addr;
	int ret = uv_ip4_addr(ip, port, &addr);
	if (ret) { return -1; }

	handle_.reset(new uv_tcp_t());
	ret = uv_tcp_init(*loop_, &*handle_);
	if (ret) { return -1; }
	handle_->data = this;

	uv_connect_t *req = new uv_connect_t();
	ret = uv_tcp_connect(req, &*handle_, (sockaddr const*)&addr, &TcpClient::S_OnConnect);
	if (ret) { return -1; }
	
	return 0;
}


int TcpClient::Close()
{
	if (!connection_) { return -1; }
	return connection_->Close();
}


void TcpClient::S_OnConnect(uv_connect_t* req, int status)
{
	TcpClient *self = (TcpClient*)req->handle->data;
	delete req;
	if (!status) {
		self->connection_.reset(new TcpConnection(std::move(self->handle_),
			std::bind(&TcpClient::OnRead, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TcpClient::OnClose, self, std::placeholders::_1)));
		self->connection_->Init();
	}
	if (self->on_connect_) { self->on_connect_(status); }
}


int TcpClient::Send(void const *data, size_t sz)
{
	if (!connection_) { return -1; }
	connection_->Send(data, sz);
	return 0;
}


void TcpClient::SetOnConnectCallback(on_connect_cb_t const cb)
{
	on_connect_ = cb;
}


void TcpClient::SetOnReadCallback(on_read_cb_t const &cb)
{
	on_read_ = cb;
}


void TcpClient::SetOnCloseCallback(on_close_cb_t const &cb)
{
	on_close_ = cb;
}


void TcpClient::OnRead(TcpConnection *connection, void *data, size_t sz)
{
	assert(connection == connection_.get());
	if (on_read_) { on_read_(peer_t(connection), data, sz); }
}


void TcpClient::OnClose(TcpConnection *connection)
{
	assert(connection == connection_.get());
	if (on_close_) { on_close_(peer_t(connection)); }
}

NS_GOKU_END
