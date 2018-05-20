#include <iostream>
#include <assert.h>
#include "file_tranfer.h"

NS_GOKU_BEG

FileTransferSender::FileTransferSender(Loop *loop, char const *file)
	: client_(loop)
	, filePath_(file)
{
	client_.SetOnConnectCallback(std::bind(&FileTransferSender::OnConnect, this, std::placeholders::_1));
	client_.SetOnReadCallback(std::bind(&FileTransferSender::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	client_.SetOnCloseCallback(std::bind(&FileTransferSender::OnClose, this, std::placeholders::_1));
}


FileTransferSender::~FileTransferSender()
{
}


int FileTransferSender::Start()
{
	int ret = client_.Connect("127.0.0.1", 50001);
	assert(!ret);
	return 0;
}


void FileTransferSender::OnRead(peer_t, void *data, size_t sz)
{
}


void FileTransferSender::OnClose(peer_t)
{
}


void FileTransferSender::OnConnect(int status)
{
	if (status) {
		std::cout << "Failed to connect to the server." << std::endl;
		return;
	}

	FILE *fp = fopen(filePath_.c_str(), "rb");
	assert(fp);
	fseek(fp, 0, SEEK_END);
	long const fsz = ftell(fp);
	assert(fsz);
	fseek(fp, 0, SEEK_SET);
	void *buf = malloc(fsz);
	size_t const read = fread(buf, 1, fsz, fp);
	assert(read == fsz);
	client_.Send(buf, fsz);
	free(buf);
}



FileTransferReceiver::FileTransferReceiver(Loop *loop)
	: server_(loop)
{
	server_.SetOnConnectionCallback(std::bind(&FileTransferReceiver::OnConnection, this, std::placeholders::_1));
	server_.SetOnReadCallback(std::bind(&FileTransferReceiver::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	server_.SetOnCloseCallback(std::bind(&FileTransferReceiver::OnClose, this, std::placeholders::_1));
}


int FileTransferReceiver::Start()
{
	int ret = server_.StartListen("0.0.0.0", 50001);
	assert(!ret);
	return 0;
}


void FileTransferReceiver::OnConnection(peer_t peer)
{
}


void FileTransferReceiver::OnRead(peer_t peer, void *buf, size_t sz)
{
	std::cout << sz << std::endl;
}


void FileTransferReceiver::OnClose(peer_t peer)
{
}

NS_GOKU_END
