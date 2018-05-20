#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "goku/define.h"
#include "goku/tcp_client.h"

NS_GOKU_BEG

class ChatClient
{
public:
	ChatClient(Loop *loop);
	
	~ChatClient();

	int Start();

private:
	void OnRead(uint64_t, void *data, size_t sz);

	void OnClose(uint64_t);

	void OnConnect(int status);

private:
	TcpClient client_;
};

NS_GOKU_END

#endif // CHAT_CLIENT_H
