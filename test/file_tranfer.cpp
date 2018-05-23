#include <iostream>
#include <assert.h>
#include "file_tranfer.h"

NS_GOKU_BEG

FileTransferSender::FileTransferSender(ILoop *loop, char const *file)
	: filePath_(file)
{
	client_ = GetGoku()->CreateTcpClient(loop);
	client_->SetOnConnectCallback(std::bind(&FileTransferSender::OnConnect, this, std::placeholders::_1));
	client_->SetOnReadCallback(std::bind(&FileTransferSender::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	client_->SetOnCloseCallback(std::bind(&FileTransferSender::OnClose, this, std::placeholders::_1));
}


FileTransferSender::~FileTransferSender()
{
	GetGoku()->DestroyTcpClient(client_);
}


int FileTransferSender::Start()
{
	int ret = client_->Connect("127.0.0.1", 50001);
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
	client_->Send(&fsz, sizeof(fsz));
	client_->Send(buf, fsz);
	free(buf);
}



FileTransferReceiver::FileTransferReceiver(ILoop *loop)
{
	server_ = GetGoku()->CreateTcpServer(loop);
	server_->SetOnConnectionCallback(std::bind(&FileTransferReceiver::OnConnection, this, std::placeholders::_1));
	server_->SetOnReadCallback(std::bind(&FileTransferReceiver::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	server_->SetOnCloseCallback(std::bind(&FileTransferReceiver::OnClose, this, std::placeholders::_1));
	
	buffer_.reserve(1 << 30);
}


FileTransferReceiver::~FileTransferReceiver()
{
	GetGoku()->DestroyTcpServer(server_);
}


int FileTransferReceiver::Start()
{
	int ret = server_->Listen("0.0.0.0", 50001);
	assert(!ret);
	return 0;
}


void FileTransferReceiver::OnConnection(peer_t peer)
{
}


void FileTransferReceiver::OnRead(peer_t peer, void *buf, size_t sz)
{
	size_t const pos = buffer_.size();
	buffer_.resize(pos + sz);
	memcpy(&buffer_[pos], buf, sz);

	// 检查文件是否接收完成
	if (buffer_.size() < sizeof(long)) { return; }
	long const fsz = *(long*)&buffer_[0];
	if (buffer_.size() < fsz + sizeof(long)) { return; }

	// 接收完成后写入文件
	FILE *fp = fopen("D:\\receiver.txt", "wb");
	assert(fp);
	fwrite(&buffer_[sizeof(long)], 1, buffer_.size() - sizeof(long), fp);
	fflush(fp);
	fclose(fp);

	// 成功接收后，断开连接
	server_->Disconnect(peer);
	
	// 停止服务器
	server_->Close();
}


void FileTransferReceiver::OnClose(peer_t peer)
{
}

NS_GOKU_END
