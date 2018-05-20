#ifndef GOKU_TCP_CLIENT_H
#define GOKU_TCP_CLIENT_H

#include "define.h"
#include "loop.h"

NS_GOKU_BEG

class TcpClientImpl;

/*
===============================================================================
===============================================================================
*/
class GOKU_API TcpClient
{
public:
	TcpClient(Loop *loop);

	TcpClient(TcpClient const&) = delete;

	TcpClient& operator=(TcpClient const&) = delete;

	~TcpClient();

	// 连接服务器
	int Connect(char const *ip, int port);

	// 与服务器断开连接
	int Disconnect();

	// 向服务器发送数据
	int Send(void *data, size_t sz);

	// 与服务端连接成功/失败
	void SetOnConnectCallback(on_connect_cb_t const cb);

	// 收到服务端数据
	void SetOnReadCallback(on_read_cb_t const &cb);

	// 与服务端连接断开
	void SetOnCloseCallback(on_close_cb_t const &cb);

private:
	TcpClientImpl *impl_;
};

NS_GOKU_END

#endif // GOKU_TCP_CLIENT_H
