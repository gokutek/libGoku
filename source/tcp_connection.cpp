#include <assert.h>
#include <algorithm>
#include "tcp_connection.h"
#include "tcp_server_impl.h"
#include "impl_define.h"

NS_GOKU_BEG

TcpConnection::TcpConnection(std::unique_ptr<uv_tcp_t> &&stream, on_read_cb_t on_read, on_close_cb_t on_close)
	: stream_(std::move(stream))
	, on_read_(on_read)
	, on_close_(on_close)
{
	stream_->data = this;
}


TcpConnection::~TcpConnection()
{
}


int TcpConnection::Init()
{
	int ret = uv_read_start((uv_stream_t*)&*stream_, &TcpConnection::S_Alloc, &TcpConnection::S_AfterRead);
	assert(ret == 0);
	return 0;
}


void TcpConnection::S_Alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = (ULONG)suggested_size;
}


void TcpConnection::S_AfterRead(uv_stream_t *stream, ssize_t nread, uv_buf_t const *buf)
{
	TcpConnection *self = (TcpConnection*)stream->data;
	assert(self != nullptr && self->stream_.get() == (uv_tcp_t*)stream);

	if (nread < 0) {
		free(buf->base);
		self->Close();
		return;
	}

	if (nread == 0) {	
		free(buf->base);
		return;
	}

	self->on_read_(self, buf->base, nread);
	free(buf->base);
}


void TcpConnection::S_AfterShutdown(uv_shutdown_t *req, int status)
{
	TcpConnection *self = (TcpConnection*)req->data;
	assert(self);
	uv_handle_t *handle = (uv_handle_t*)req->handle;
	delete req;
	uv_close(handle, &TcpConnection::S_OnClose);	
}


int TcpConnection::Send(void const *data, size_t sz)
{
	if (!data) { return -1; }

	bool const writeNow = writeQueue_.empty();

	while (sz > 0) {
		size_t const frame_sz = (std::min)(sz, size_t(65536));
		sz -= frame_sz;
		auto req = std::make_unique<WriteRequest>();
		void *clone = malloc(frame_sz);
		memcpy(clone, data, frame_sz);
		data = (char const*)data + frame_sz;
		req->buf = uv_buf_init((char*)clone, (unsigned int)frame_sz);
		req->req.data = this;
		writeQueue_.push(std::move(req));
	}

	if (writeNow) { SendFromQueue(); }

	return 0;
}


void TcpConnection::SendFromQueue()
{
	assert(!writeQueue_.empty());
	WriteRequest *req = writeQueue_.front().get();
	int ret = uv_write(&req->req, (uv_stream_t*)stream_.get(), &req->buf, 1, &TcpConnection::S_AfterWrite);
	assert(ret == 0);
}


void TcpConnection::S_AfterWrite(uv_write_t *req, int status)
{
	assert(!status);
	TcpConnection *self = (TcpConnection*)req->data;
	assert(self);
	assert(!self->writeQueue_.empty());
	assert(&self->writeQueue_.front().get()->req == req);
	self->writeQueue_.pop();
	if (!self->writeQueue_.empty() && uv_is_writable((uv_stream_t*)self->stream_.get())) {
		self->SendFromQueue();
	}
}


void TcpConnection::S_OnClose(uv_handle_t* handle)
{
	TcpConnection *self = (TcpConnection*)handle->data;
	assert(self);
	self->on_close_(self);
}


int TcpConnection::Close()
{
	uv_stream_t *stream = (uv_stream_t*)stream_.get();

	if (uv_is_readable(stream)) {
		uv_read_stop((uv_stream_t*)stream_.get());
	}

	if (uv_is_writable(stream)) {
		uv_shutdown_t *sreq = new uv_shutdown_t();
		sreq->data = stream->data;
		uv_shutdown(sreq, stream, &TcpConnection::S_AfterShutdown);
	}

	return 0;
}

NS_GOKU_END
