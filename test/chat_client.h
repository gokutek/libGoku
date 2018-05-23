#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "goku/loader.h"
#include "goku/goku.h"

NS_GOKU_BEG

class ChatClient
{
public:
	ChatClient(ILoop *loop);
	
	~ChatClient();

	int Start();

private:
	void OnRead(peer_t, void *data, size_t sz);

	void OnClose(peer_t);

	void OnConnect(int status);

private:
	ITcpClient *client_;
};

NS_GOKU_END

#endif // CHAT_CLIENT_H
