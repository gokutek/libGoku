#include <assert.h>
#include <crtdbg.h>
#include <iostream>
#include "goku/loader.h"
#include "goku/goku.h"
#include "echo_server.h"
#include "chat_client.h"
#include "file_tranfer.h"


static void test_echo_server()
{
	goku::ILoop *loop = GetGoku()->CreateLoop();
	goku::EchoServer server(loop);
	server.Start();
	loop->Run(goku::LoopRunMode::DEFAULT);
	loop->Close();
	GetGoku()->DestroyLoop(loop);
}


static void test_chat_client()
{
	goku::ILoop *loop = GetGoku()->CreateLoop();
	goku::ChatClient client(loop);
	client.Start();
	loop->Run(goku::LoopRunMode::DEFAULT);
	loop->Close();
	GetGoku()->DestroyLoop(loop);
}


static void test_file_transfer_sender(char const *file)
{
	goku::ILoop *loop = GetGoku()->CreateLoop();
	goku:: FileTransferSender sender(loop, file);
	sender.Start();
	loop->Run(goku::LoopRunMode::DEFAULT);
	loop->Close();
	GetGoku()->DestroyLoop(loop);
}


static void test_file_transfer_receiver()
{
	goku::ILoop *loop = GetGoku()->CreateLoop();
	goku::FileTransferReceiver server(loop);
	server.Start();
	loop->Run(goku::LoopRunMode::DEFAULT);
	loop->Close();
	GetGoku()->DestroyLoop(loop);
}


int console_main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Invalid command line" << std::endl;
		return 1;
	}

	std::string const cmd = argv[1];
	if (cmd == "test_echo_server") {
		test_echo_server();
	} else if (cmd == "test_chat_client") {
		test_chat_client();
	} else if (cmd == "test_file_transfer_receiver") {
		test_file_transfer_receiver();
	} else if (cmd == "test_file_transfer_sender") {
		if (argc != 3) {
			std::cerr << "Please specified the file path you want to trasfer" << std::endl;
			return 1;
		}
		test_file_transfer_sender(argv[2]);
	}
	return 0;
}


int main(int argc, char *argv[])
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	//_CrtSetBreakAlloc(163);
	int const ret = console_main(argc, argv);
	_CrtDumpMemoryLeaks();
	return ret;
}
