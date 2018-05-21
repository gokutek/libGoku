#include <windows.h>
#include "goku/goku.h"
#include "object_factory.h"


goku::IObjectFactory* GetObjectFactory()
{
	static goku::ObjectFactory factory;
	return &factory;
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}
