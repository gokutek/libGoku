#ifndef GOKU_LOADER
#define GOKU_LOADER

#include <windows.h>
#include <tchar.h>
#include "i_object_factory.h"

/*
===============================================================================
运行时动态加载libGoku.dll，并返回对象厂类接口。
客户程序不需要再link静态库。
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
