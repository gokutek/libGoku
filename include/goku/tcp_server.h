#ifndef GOKU_TCP_SERVER_H
#define GOKU_TCP_SERVER_H

#include <functional>
#include "define.h"
#include "loop.h"

NS_GOKU_BEG

class TcpServerImpl;

class GOKU_API TcpServer
{
public:
	TcpServer(Loop *loop);

	TcpServer(TcpServer const&) = delete;

	TcpServer& operator=(TcpServer const&) = delete;

	~TcpServer();

	// 开始监听
	int StartListen(char const *ip, int port);

	// 停止监听
	int StopListen();

	// 收到客户端连接
	void SetOnConnectionCallback(on_connection_cb_t const cb);

	// 收到客户端数据
	void SetOnReadCallback(on_read_cb_t const &cb);

	// 客户端连接断开
	void SetOnCloseCallback(on_close_cb_t const &cb);

	// 向客户端发送数据
	int Send(uint64_t peer, void *data, size_t sz);

	// 断开与客户端的连接
	int Disconnect(uint64_t peer);

private:
	TcpServerImpl *impl_;
};

NS_GOKU_END

#endif // GOKU_TCP_SERVER_H
