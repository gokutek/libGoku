#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <memory>
#include "goku/define.h"
#include "goku/i_tcp_client.h"
#include "loop.h"
#include "uv.h"

NS_GOKU_BEG

class TcpConnection;

class TcpClient : public ITcpClient
{
public:
	TcpClient(Loop *loop);
	
	~TcpClient();

	int Connect(char const *ip, int port) override;

	int Close() override;

	int Send(void const *data, size_t sz) override;

	void SetOnConnectCallback(on_connect_cb_t const cb) override;

	void SetOnReadCallback(on_read_cb_t const &cb) override;

	void SetOnCloseCallback(on_close_cb_t const &cb) override;

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

#endif // TCP_CLIENT_H
