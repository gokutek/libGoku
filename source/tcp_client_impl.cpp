#include <assert.h>
#include "tcp_client_impl.h"
#include "tcp_connection.h"

NS_GOKU_BEG

TcpClientImpl::TcpClientImpl(Loop *loop)
	: loop_(loop)
{
}


TcpClientImpl::~TcpClientImpl()
{
}


int TcpClientImpl::Connect(char const *ip, int port)
{
	struct sockaddr_in addr;
	int ret = uv_ip4_addr(ip, port, &addr);
	if (ret) { return -1; }

	handle_.reset(new uv_tcp_t());
	ret = uv_tcp_init(*loop_, &*handle_);
	if (ret) { return -1; }
	handle_->data = this;

	uv_connect_t *req = new uv_connect_t();
	ret = uv_tcp_connect(req, &*handle_, (sockaddr const*)&addr, &TcpClientImpl::S_OnConnect);
	if (ret) { return -1; }
	
	return 0;
}


int TcpClientImpl::Disconnect()
{
	if (!connection_) { return -1; }
	return connection_->Close();
}


void TcpClientImpl::S_OnConnect(uv_connect_t* req, int status)
{
	TcpClientImpl *self = (TcpClientImpl*)req->handle->data;
	delete req;
	if (!status) {
		self->connection_.reset(new TcpConnection(std::move(self->handle_),
			std::bind(&TcpClientImpl::OnRead, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TcpClientImpl::OnClose, self, std::placeholders::_1)));
		self->connection_->Init();
	}
	if (self->on_connect_) { self->on_connect_(status); }
}


int TcpClientImpl::Send(void *data, size_t sz)
{
	if (!connection_) { return -1; }
	connection_->Send(data, sz);
	return 0;
}


void TcpClientImpl::SetOnConnectCallback(on_connect_cb_t const cb)
{
	on_connect_ = cb;
}


void TcpClientImpl::SetOnReadCallback(on_read_cb_t const &cb)
{
	on_read_ = cb;
}


void TcpClientImpl::SetOnCloseCallback(on_close_cb_t const &cb)
{
	on_close_ = cb;
}


void TcpClientImpl::OnRead(TcpConnection *connection, void *data, size_t sz)
{
	assert(connection == connection_.get());
	if (on_read_) { on_read_(uint64_t(connection), data, sz); }
}


void TcpClientImpl::OnClose(TcpConnection *connection)
{
	assert(connection == connection_.get());
	if (on_close_) { on_close_(uint64_t(connection)); }
}

NS_GOKU_END
