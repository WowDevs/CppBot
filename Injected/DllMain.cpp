#pragma once
#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <string>
#include <cstdio>

#include "WindowsConsole.hpp"
#include "Manager.hpp"
#include "KeyboardHook.hpp"

HINSTANCE instanceDLL = NULL;

DWORD MainThreadControl(LPVOID /* param */);
LRESULT WINAPI KeyboardHook(int/* nCode*/, WPARAM/* wParam*/, LPARAM/* lParam*/);

// entry point of the DLL
BOOL APIENTRY DllMain(HINSTANCE instDLL, DWORD reason, LPVOID /* reserved */)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		instanceDLL = instDLL;
		// disables thread notifications (DLL_THREAD_ATTACH, DLL_THREAD_DETACH)
		DisableThreadLibraryCalls(instDLL);		
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThreadControl, NULL, 0, NULL);		
	}	
	else if (reason == DLL_PROCESS_DETACH)
	{		
		WindowsConsole::Destroy();
		
	}
	return TRUE;
}

int endScenePointer = 0;
int resetPointer = 0;
void GetDevicePointers();

void WINAPI MessageLoop(LPVOID lpParm)
{
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, instanceDLL, 0);
	MSG message;
	GetMessage(&message, NULL, 0, 0);	
}

DWORD MainThreadControl(LPVOID lpParm)
{	
	WindowsConsole::Create();	
	Sleep(200);	

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MessageLoop, NULL, 0, NULL);

	GetDevicePointers();
	printf("End Scene Pointer = %X\n", endScenePointer); 
	printf("Reset Pointer = %X\n", resetPointer);

	g_Detours["EndScene"] = new Detour(endScenePointer, (int)EndSceneDetour);
	g_Detours["Reset"] = new Detour(resetPointer, (int)ResetDetour);
	//g_Detours["OnKeyUp"] = new Detour(0x00763BE0, (int)OnKeyUpDetour);		
	

	while (!should_exit)
	{		
	}	

	if (hKeyHook)	
		UnhookWindowsHookEx(hKeyHook);

	shouldRemoveEndSceneInjection = true;

	printf("Exiting...\n");
	//system("pause");

	while (!endSceneUnhooked) { }

	// unhooks functions
	for (auto& det : g_Detours)
	{
		delete det.second;
	}
	g_Detours.clear();
	// shutdowns the bot
	// note: after that DLL's entry point will be called with reason DLL_PROCESS_DETACH
	FreeLibraryAndExitThread((HMODULE)instanceDLL, 0);
	return 0;
}

void GetDevicePointers()
{
	// Device
	int ptr;
	{
		ptr = *(int*)0x00C5DF88;
	}while (ptr == 0);

	{
		ptr = *(int*)(ptr + 0x397C);
	}while (ptr == 0);

	// Scene	
	{
		ptr = *(int*)ptr;
	}while (ptr == 0);

	{
		endScenePointer = *(int*)(ptr + 0xA8);   //42 * 4	
	}while (endScenePointer == 0);

	{
		resetPointer = *(int*)(ptr + 0x40);   //16 * 4	
	}while (resetPointer == 0);
}







































//hKeyHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardHook, instanceDLL, 0);
//if (hKeyHook)
//	UnhookWindowsHookEx(hKeyHook);


//int countt;
//int __stdcall EndSceneDetour(int device)
//{
//	if (countt++ % 10 == 0)
//	{
//		//MessageBox(HWND_DESKTOP, "Text here", "Text here", MB_OK);
//		printf("count = %d\n", countt);
//		if (countt < 30)
//		{
//			((void(__cdecl*)(const char*, const char*, void*))0x00819210)("print('aaaaaaaaaaa')", "LetsDance", nullptr);
//		}
//	}
//	auto det = g_Detours["EndScene"];
//	det->Restore();
//	int res = ((int(__stdcall*)(int))det->GetOrig())(device);
//	det->Apply();
//	return res;
//}