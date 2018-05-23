#ifndef GOKU_I_TCP_CLIENT_H
#define GOKU_I_TCP_CLIENT_H

#include "define.h"

NS_GOKU_BEG

/*
===============================================================================
===============================================================================
*/
class ITcpClient
{
public:
	virtual ~ITcpClient() = default;

	// 连接服务器
	virtual int Connect(char const *ip, int port) = 0;

	// 与服务器断开连接
	virtual int Close() = 0;

	// 向服务器发送数据
	virtual int Send(void const *data, size_t sz) = 0;

	// 设置与服务端连接成功/失败回调
	virtual void SetOnConnectCallback(on_connect_cb_t const cb) = 0;

	// 设置收到服务端数据回调
	virtual void SetOnReadCallback(on_read_cb_t const &cb) = 0;

	// 设置与服务端连接断开回调
	virtual void SetOnCloseCallback(on_close_cb_t const &cb) = 0;
};

NS_GOKU_END

#endif // GOKU_I_TCP_CLIENT_H
