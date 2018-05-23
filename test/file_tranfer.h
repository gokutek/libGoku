#ifndef FILE_TRANFER_H
#define FILE_TRANFER_H

#include <string>
#include <vector>
#include "goku/loader.h"
#include "goku/goku.h"

NS_GOKU_BEG

class FileTransferSender
{
public:
	FileTransferSender(ILoop *loop, char const *file);

	~FileTransferSender();

	int Start();

private:
	void OnRead(peer_t, void *data, size_t sz);

	void OnClose(peer_t);

	void OnConnect(int status);

private:
	ITcpClient	*client_;
	std::string	filePath_;
};


class FileTransferReceiver
{
public:
	FileTransferReceiver(ILoop *loop);

	~FileTransferReceiver();

	int Start();

private:
	void OnConnection(peer_t peer);

	void OnRead(peer_t peer, void *buf, size_t sz);

	void OnClose(peer_t peer);

private:
	ITcpServer			*server_;
	std::vector<char>	buffer_;
};

NS_GOKU_END

#endif // FILE_TRANFER_H
