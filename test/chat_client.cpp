#include <assert.h>
#include <iostream>
#include <string>
#include "chat_client.h"

NS_GOKU_BEG

ChatClient::ChatClient(ILoop *loop)
{
	client_ = GetGoku()->CreateTcpClient(loop);
	client_->SetOnConnectCallback(std::bind(&ChatClient::OnConnect, this, std::placeholders::_1));
	client_->SetOnReadCallback(std::bind(&ChatClient::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	client_->SetOnCloseCallback(std::bind(&ChatClient::OnClose, this, std::placeholders::_1));
}


ChatClient::~ChatClient()
{
	GetGoku()->DestroyTcpClient(client_);
}


int ChatClient::Start()
{
	int ret = client_->Connect("127.0.0.1", 9005);
	assert(!ret);
	return 0;
}


void ChatClient::OnRead(peer_t, void *data, size_t sz)
{
	std::string const str((char const*)data, (char const*)data + sz);
	std::cout << str << std::endl;
	if (str == "quit") {
		client_->Disconnect();
	}
}


void ChatClient::OnClose(peer_t)
{
	std::cout << "Lost connection to the server" << std::endl;
}


void ChatClient::OnConnect(int status)
{
	std::cout << "Connect to server finish, errno = " << status << std::endl;
}

NS_GOKU_END
