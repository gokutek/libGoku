#include "goku/tcp_client.h"
#include "tcp_client_impl.h"

NS_GOKU_BEG

TcpClient::TcpClient(Loop *loop)
	: impl_(new TcpClientImpl(loop))
{
}


TcpClient::~TcpClient()
{
	delete impl_;
}


int TcpClient::Connect(char const *ip, int port)
{
	return impl_->Connect(ip, port);
}


int TcpClient::Disconnect()
{
	return impl_->Disconnect();
}


int TcpClient::Send(void const *data, size_t sz)
{
	return impl_->Send(data, sz);
}


void TcpClient::SetOnConnectCallback(on_connect_cb_t const cb)
{
	impl_->SetOnConnectCallback(cb);
}


void TcpClient::SetOnReadCallback(on_read_cb_t const &cb)
{
	impl_->SetOnReadCallback(cb);
}


void TcpClient::SetOnCloseCallback(on_close_cb_t const &cb)
{
	impl_->SetOnCloseCallback(cb);
}

NS_GOKU_END
