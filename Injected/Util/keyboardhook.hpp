#pragma once
#include "Util\Helpers.hpp"
#include "..\Manager.hpp"

bool should_exit = false;

#include <Windows.h>

HHOOK hKeyHook = nullptr;


LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{

	}

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP)))
	{
		auto key = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;

		//printf("%X", key);

		if (key == VK_NUMPAD7)
			should_exit = true;
	}

	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

bool __cdecl OnKeyUpDetour(int* key)
{	
	//chat(tohex(*key));
	
	/*if (*key == 0x107)
	{
		should_exit = true;
		
	}*/

	auto det = g_Detours["OnKeyUp"];
	det->Restore(); 
	auto res = ((bool(__cdecl*)(int*))det->GetOrig())(key);
	det->Apply();
	return res;
}