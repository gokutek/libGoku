#ifndef GOKU_I_OBJECT_FACTORY_H
#define GOKU_I_OBJECT_FACTORY_H

#include "define.h"

NS_GOKU_BEG

class ILoop;
class ITcpClient;
class ITcpServer;

class IObjectFactory
{
public:
	virtual ~IObjectFactory() = default;

	virtual ILoop* CreateLoop() = 0;
	virtual void DestroyLoop(ILoop *obj) = 0;
	
	virtual ITcpClient* CreateTcpClient(ILoop *loop) = 0;
	virtual void DestroyTcpClient(ITcpClient *obj) = 0;

	virtual ITcpServer* CreateTcpServer(ILoop *loop) = 0;
	virtual void DestroyTcpServer(ITcpServer *obj) = 0;
};

NS_GOKU_END

#endif // GOKU_I_OBJECT_FACTORY_H
