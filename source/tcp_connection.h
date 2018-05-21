#ifndef GOKU_TCP_CONNECTION_H
#define GOKU_TCP_CONNECTION_H

#include <functional>
#include <memory>
#include <queue>
#include "uv.h"
#include "goku/define.h"
#include "impl_define.h"

NS_GOKU_BEG

class TcpConnection
{
	typedef std::function<void(TcpConnection*, void*, size_t)> on_read_cb_t;
	typedef std::function<void(TcpConnection*)> on_close_cb_t;

public:
	TcpConnection(std::unique_ptr<uv_tcp_t> &&stream, on_read_cb_t on_read, on_close_cb_t on_close);
	
	~TcpConnection();

	int Init();

	int Send(void const *data, size_t sz);

	int Close();

private:
	static void S_Alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void S_AfterRead(uv_stream_t *stream, ssize_t nread, uv_buf_t const *buf);
	static void S_AfterShutdown(uv_shutdown_t *req, int status);
	static void S_AfterWrite(uv_write_t *req, int status);
	static void S_OnClose(uv_handle_t* handle);

	void SendFromQueue();

private:
	std::unique_ptr<uv_tcp_t>					stream_;
	on_read_cb_t								on_read_;
	on_close_cb_t								on_close_;
	std::queue<std::unique_ptr<WriteRequest>>	writeQueue_;
};

NS_GOKU_END

#endif // GOKU_TCP_CONNECTION_H
