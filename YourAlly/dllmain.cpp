// dllmain.cpp: определяет точку входа для приложения DLL.
#include "stdafx.h"
#include "YourAlly.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

bool indicator = false;
bool triger = false;
bool glowesp = true;
//offsets
#define dwLocalPlayer			0xCCA6A4
#define dwEntityList			0x4CDB00C
#define dwGlowObjectManager		0x521AF50

#define m_bSpotted		0x93C
#define m_iHealth		0x100
#define m_iTeamNum		0xF4
#define m_iGlowIndex	0xA3F8
#define m_iCrosshairId  0xB394

typedef struct GlowObjectDefinition_t 
{
	float r;
	float g;
	float b;
	float a;
	uint8_t unkl[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	int GlowStyle;
	uint8_t unk2[10];
} GlowObjectDefinition_t;

int RandInt(int a, int b)//1,6 | 10,6
{
	return (a + rand() % b);
}



DWORD __stdcall Menu(void* lpParam)
{
	while (TRUE)
	{
		if (GetAsyncKeyState(VK_F2))
			indicator = !indicator;
		
		if (GetAsyncKeyState(VK_F2)) //t o
			triger = !triger;
			
		if (GetAsyncKeyState(VK_F2))// g o
			glowesp = !glowesp;
		
		Sleep(40);
	}
}

DWORD __stdcall RadarHack(void* lpParam)
{
	memory mem;
	PModule InfoModule;
	InfoModule = mem.GetModuls();
	DWORD CSmodule = InfoModule.СlientBase;

	while (TRUE)
	{
		while (triger)
		{
			DWORD pLocal = *reinterpret_cast<DWORD*>(CSmodule + dwLocalPlayer);//0xCBD6B4
			if (!pLocal)
				continue;

			int pLocalHp = *reinterpret_cast<int*>(pLocal + m_iHealth);// 0x100
			if (!pLocalHp)
				continue;

			int pLocalTeam = *reinterpret_cast<int*>(pLocal + m_iTeamNum);// 0xF4
			if (!pLocalTeam)
				continue;

			for (int i = 0; i < 20; i++)
			{
				DWORD pEntity = *reinterpret_cast<DWORD*>(CSmodule + dwEntityList + i * 0x10);
				if (!pEntity)
					continue;
				
				int EntityHp = *reinterpret_cast<int*>(pEntity + m_iHealth);
				if (!EntityHp)
					continue;
				
				int EntityTeam = *reinterpret_cast<int*>(pEntity + m_iTeamNum);
				if (!EntityTeam)
					continue;
				
				if (EntityTeam == pLocalTeam)
					continue;
				
				int EntitySpotted = *reinterpret_cast<int*>(pEntity + m_bSpotted);
				if (EntitySpotted == 0x100 || EntitySpotted >= 0x110 || EntitySpotted <= -1)
					continue;

				*reinterpret_cast<int*>(pEntity + m_bSpotted) = 0x100;
				Sleep(1);
			}
			Sleep(1);		
		}
		Sleep(100);
	}
}

DWORD __stdcall GlowEsp(void* lpParam)
{
	memory mem;
	PModule InfoModule;
	InfoModule = mem.GetModuls();
	DWORD CSmodule = InfoModule.СlientBase;
	DWORD ModuleSize = InfoModule.СlientBase + InfoModule.SizeDll;

	while (TRUE)
	{
		while (glowesp)
		{
			CSmodule = 0;
			InfoModule = mem.GetModuls();
			CSmodule = InfoModule.СlientBase;
			if (CSmodule == 0)
				break;

			DWORD pLocal = *reinterpret_cast<DWORD*>(CSmodule + dwLocalPlayer);//0xCBD6B4
			if (!pLocal)
				continue;

			int pLocalHp = *reinterpret_cast<int*>(pLocal + m_iHealth);// 0x100
			if (!pLocalHp)
				continue;

			int pLocalTeam = *reinterpret_cast<int*>(pLocal + m_iTeamNum);// 0xF4
			if (!pLocalTeam)
				continue;

			DWORD GlowArray = *reinterpret_cast<DWORD*>(CSmodule + dwGlowObjectManager);
			if (!GlowArray)
				continue;
			int GlowCount = *reinterpret_cast<int*>(CSmodule + dwGlowObjectManager + 0x04);
			if (!GlowCount)
				continue;

			for (int i = 0; i < GlowCount-1; i++)
			{
				DWORD gEntity = *reinterpret_cast<DWORD*>(GlowArray + 0x38 * i);//4,1,7
				if (!(gEntity > CSmodule)) //2 6
					continue;
				int eHP = *reinterpret_cast<int*>(gEntity + m_iHealth);
				if (!eHP)
					continue;
				int eTeamNum = *reinterpret_cast<int*>(gEntity + m_iTeamNum);
				if (!eTeamNum)
					continue;
				if (eTeamNum == pLocalTeam)
					continue;
				static GlowObjectDefinition_t Glow = *reinterpret_cast<GlowObjectDefinition_t*>(GlowArray + (0x38 * i) + 0x04);
				if (eTeamNum == 3)
				{
					Glow.r = 0.447058827f;
					Glow.g = 0.600023213f;
					Glow.b = 0.867125f;
					Glow.a = 0.9f;
					Glow.m_bRenderWhenOccluded = true;
					Glow.m_bRenderWhenUnoccluded = false;
				}
				if (eTeamNum == 2)
				{
					Glow.r = 0.877058827f;
					Glow.g = 0.340023213f;
					Glow.b = 0.337125f;
					Glow.a = 0.9f;
					Glow.m_bRenderWhenOccluded = true;
					Glow.m_bRenderWhenUnoccluded = false;
				}
				*reinterpret_cast<GlowObjectDefinition_t*>(GlowArray + (0x38 * i) + 0x04) = Glow;
				Sleep(1);
			}
			Sleep(1);
		}
		Sleep(100);
	}
}

DWORD __stdcall Trigger(void* lpParam)
{
	memory mem;
	PModule InfoModule;
	InfoModule = mem.GetModuls();
	DWORD CSmodule = InfoModule.СlientBase;

	while (TRUE)
	{
		while (indicator)
		{
			DWORD pLocal = *reinterpret_cast<DWORD*>(CSmodule + dwLocalPlayer);//0xCBD6B4
			if (!pLocal)
				continue;

			int pLocalHp = *reinterpret_cast<int*>(pLocal + m_iHealth);// 0x100
			if (!pLocalHp)
				continue;

			int pLocalTeam = *reinterpret_cast<int*>(pLocal + m_iTeamNum);// 0xF4
			if (!pLocalTeam)
				continue;

			DWORD CrosshairId = *reinterpret_cast<DWORD*>(pLocal + m_iCrosshairId);
			if (!CrosshairId || CrosshairId > 64)
				continue;

			DWORD pEntity = *reinterpret_cast<DWORD*>(CSmodule + dwEntityList + (CrosshairId - 1) * 0x10);
			if (!pEntity)
				continue;

			int entityHP = *reinterpret_cast<int*>(pEntity + m_iHealth);
			if (!entityHP)
				continue;

			int entityTeam = *reinterpret_cast<int*>(pEntity + m_iTeamNum);
			if (!entityTeam)
				continue;

			if (entityTeam == pLocalTeam)
				continue;

			//SHOT
			Sleep(RandInt(1, 6));
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(RandInt(10, 6));
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(100);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//MessageBox(NULL, "susesfull", "susesfulllololol", MB_OK);
		Sleep(10000); 
		//CreateThread(0, 0, &Menu, 0, 0, 0);
		//CreateThread(0, 0, &RadarHack, 0, 0, 0);
		CreateThread(0, 0, &GlowEsp, 0, 0, 0);
		//CreateThread(0, 0, &Trigger, 0, 0, 0);
    case DLL_THREAD_ATTACH:

    case DLL_THREAD_DETACH:

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
