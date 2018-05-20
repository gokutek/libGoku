#ifndef FILE_TRANFER_H
#define FILE_TRANFER_H

#include <string>
#include "goku/tcp_server.h"
#include "goku/tcp_client.h"

NS_GOKU_BEG

class FileTransferSender
{
public:
	FileTransferSender(Loop *loop, char const *file);

	~FileTransferSender();

	int Start();

private:
	void OnRead(peer_t, void *data, size_t sz);

	void OnClose(peer_t);

	void OnConnect(int status);

private:
	TcpClient	client_;
	std::string	filePath_;
};


class FileTransferReceiver
{
public:
	FileTransferReceiver(Loop *loop);

	int Start();

private:
	void OnConnection(peer_t peer);

	void OnRead(peer_t peer, void *buf, size_t sz);

	void OnClose(peer_t peer);

private:
	TcpServer server_;
};

NS_GOKU_END

#endif // FILE_TRANFER_H
