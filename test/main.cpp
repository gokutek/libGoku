#include <crtdbg.h>
#include <iostream>
#include "echo_server.h"
#include "goku/loop.h"

void server_main()
{
	goku::Loop loop;
	goku::EchoServer server(&loop);
	server.Start();
	loop.Run(LoopRunMode::DEFAULT);

	loop.Close();
}


int main()
{
	server_main();
	_CrtDumpMemoryLeaks();
	return 0;
}
