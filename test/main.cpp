#include <assert.h>
#include <crtdbg.h>
#include <iostream>
#include "goku/loop.h"
#include "echo_server.h"
#include "chat_client.h"
#include "file_tranfer.h"


static void test_echo_server()
{
	goku::Loop loop;
	goku::EchoServer server(&loop);
	server.Start();
	loop.Run(goku::LoopRunMode::DEFAULT);
	loop.Close();
}


static void test_chat_client()
{
	goku::Loop loop;
	goku::ChatClient client(&loop);
	client.Start();
	loop.Run(goku::LoopRunMode::DEFAULT);
	loop.Close();
}


static void test_file_transfer_sender(char const *file)
{
	goku::Loop loop;
	goku:: FileTransferSender sender(&loop, file);
	sender.Start();
	loop.Run(goku::LoopRunMode::DEFAULT);
	loop.Close();
}


static void test_file_transfer_receiver()
{
	goku::Loop loop;
	goku::FileTransferReceiver server(&loop);
	server.Start();
	loop.Run(goku::LoopRunMode::DEFAULT);
	loop.Close();
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
	int const ret = console_main(argc, argv);
	_CrtDumpMemoryLeaks();
	return ret;
}
