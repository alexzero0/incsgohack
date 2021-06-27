#pragma once
// YourAlly.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <windows.h>
#include <Psapi.h>




struct PModule
{
	DWORD СlientBase;
	DWORD SizeDll;//возможно лишнее
};


class memory
{
public:
	inline PModule GetModuls()
	{
		MODULEINFO GameModule = { 0 };
		HMODULE ClientModule1 = GetModuleHandleA("client_panorama.dll");
		GetModuleInformation(GetCurrentProcess(), ClientModule1, &GameModule, sizeof(MODULEINFO));
		return PModule{ reinterpret_cast<DWORD>(GameModule.lpBaseOfDll), GameModule.SizeOfImage };
	}
	
};
