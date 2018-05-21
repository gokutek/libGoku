#ifndef GOKU_LOADER
#define GOKU_LOADER

#include <windows.h>
#include <tchar.h>
#include "i_object_factory.h"

/*
===============================================================================
����ʱ��̬����libGoku.dll�������ض�����ӿڡ�
�ͻ�������Ҫ��link��̬�⡣
===============================================================================
*/
inline goku::IObjectFactory* GetGoku()
{
	HMODULE hModule = LoadLibraryW(L"libGoku.dll");
	if (NULL == hModule) { return nullptr; }
	auto pfn = (goku::IObjectFactory*(*)())GetProcAddress(hModule, "GetObjectFactory");
	if (!pfn) { return nullptr; }
	return pfn();
}

#endif // GOKU_LOADER
