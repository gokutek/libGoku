#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "goku/i_object_factory.h"

NS_GOKU_BEG

class ObjectFactory : public IObjectFactory
{
public:
	ILoop* CreateLoop() override;
	void DestroyLoop(ILoop *obj) override;

	ITcpClient* CreateTcpClient(ILoop *loop) override;
	void DestroyTcpClient(ITcpClient *obj) override;

	ITcpServer* CreateTcpServer(ILoop *loop) override;
	void DestroyTcpServer(ITcpServer *obj) override;
};

NS_GOKU_END

#endif // OBJECT_FACTORY_H
