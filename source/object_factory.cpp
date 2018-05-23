#include "object_factory.h"
#include "loop.h"
#include "tcp_client.h"
#include "tcp_server.h"

NS_GOKU_BEG

ILoop* ObjectFactory::CreateLoop()
{
	return new Loop();
}


void ObjectFactory::DestroyLoop(ILoop *obj)
{
	delete obj;
}


ITcpClient* ObjectFactory::CreateTcpClient(ILoop *loop)
{
	return new TcpClient(dynamic_cast<Loop*>(loop));
}


void ObjectFactory::DestroyTcpClient(ITcpClient *obj)
{
	delete obj;
}


ITcpServer* ObjectFactory::CreateTcpServer(ILoop *loop)
{
	return new TcpServer(dynamic_cast<Loop*>(loop));
}


void ObjectFactory::DestroyTcpServer(ITcpServer *obj)
{
	delete obj;
}

NS_GOKU_END
