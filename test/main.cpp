#include <crtdbg.h>
#include <iostream>
#include "goku/loop.h"
#include "echo_server.h"
#include "chat_client.h"


static void test_echo_server()
{
	goku::Loop loop;
	goku::EchoServer server(&loop);
	server.Start();
	loop.Run(LoopRunMode::DEFAULT);
	loop.Close();
}


static void test_chat_client()
{
	goku::Loop loop;
	goku::ChatClient server(&loop);
	server.Start();
	loop.Run(LoopRunMode::DEFAULT);
	loop.Close();
}


int main()
{
	//test_echo_server();
	test_chat_client();
	_CrtDumpMemoryLeaks();
	return 0;
}
