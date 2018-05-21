#ifndef TCP_CLIENT_IMPL_H
#define TCP_CLIENT_IMPL_H

#include <memory>
#include "goku/define.h"
#include "goku/loop.h"
#include "uv.h"

NS_GOKU_BEG

class TcpConnection;

class TcpClientImpl
{
public:
	TcpClientImpl(Loop *loop);
	
	~TcpClientImpl();

	int Connect(char const *ip, int port);

	int Disconnect();

	int Send(void const *data, size_t sz);

	void SetOnConnectCallback(on_connect_cb_t const cb);

	void SetOnReadCallback(on_read_cb_t const &cb);

	void SetOnCloseCallback(on_close_cb_t const &cb);

private:
	static void S_OnConnect(uv_connect_t* req, int status);

	void OnRead(TcpConnection *connection, void *data, size_t sz);

	void OnClose(TcpConnection *connection);

private:
	Loop							*loop_;
	std::unique_ptr<uv_tcp_t>		handle_;
	std::unique_ptr<TcpConnection>	connection_;
	on_connect_cb_t					on_connect_;
	on_read_cb_t					on_read_;
	on_close_cb_t					on_close_;
};

NS_GOKU_END

#endif // TCP_CLIENT_IMPL_H
