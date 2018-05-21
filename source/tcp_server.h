#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <map>
#include <memory>
#include <stdint.h>
#include "goku/define.h"
#include "goku/i_tcp_server.h"
#include "loop.h"
#include "uv.h"

NS_GOKU_BEG

class TcpConnection;

class TcpServer : public ITcpServer
{
public:
	TcpServer(Loop *loop);

	~TcpServer();

	int StartListen(char const *ip, int port) override;

	int StopListen() override;

	void SetOnConnectionCallback(on_connection_cb_t const cb) override;

	void SetOnReadCallback(on_read_cb_t const &cb) override;

	void SetOnCloseCallback(on_close_cb_t const &cb) override;

	int Send(peer_t peer, void const *data, size_t sz) override;

	int Disconnect(peer_t peer) override;

private:
	// 收到客户端连接
	static void S_OnConnection(uv_stream_t* server, int status);
	
	// 监听套接字关闭
	static void S_OnClose(uv_handle_t* handle);

	void OnConnection(uv_stream_t* server, int status);

	void OnRead(TcpConnection *connection, void *buf, size_t sz);

	void OnClose(TcpConnection *connection);
	
private:
	Loop												*loop_;
	std::unique_ptr<uv_tcp_t>							server_;
	std::map<peer_t, std::unique_ptr<TcpConnection>>	clients_;
	on_connection_cb_t									on_connection_;
	on_read_cb_t										on_read_;
	on_close_cb_t										on_close_;
};

NS_GOKU_END

#endif // TCP_SERVER_H
