#pragma once
#include <map>
#include <string>
#include <d3d9.h>
#include "D3dx9core.h"
#include "Memory\Detour.hpp"
#include "Util\Timer.hpp"
#include "Util\Helpers.hpp"
#include "Lua\Lua.hpp"
#include "WowStuff\WowFuncs.hpp"
#include "Objects\Spell.hpp"
#include "Routines\PaladinLeveling.hpp"
#include "Graphics.hpp"

bool shouldRemoveEndSceneInjection = false;
bool endSceneUnhooked = false;
std::map<std::string, Detour*> g_Detours;
bool should_exit = false;
bool do_something = false;

int frameCount = 0;
Graphics* graph = nullptr;
timer afkTimer;
int silly_counter = 0;

void mrint(std::string name, std::string value)
{
	if (graph == nullptr)
		return;

	std::string line = name + " = " + value;
	graph->AddTextLine(line);
}

//---------------- END SCENE DETOUR ------------------
int __stdcall EndSceneDetour(LPDIRECT3DDEVICE9 device)
{
	OM_Pulse();

	if (frameCount % 30 == 0)
	{
		//printf("%d ", frameCount);
		printf(".");
	}
	if (frameCount == 0)
		chat("CppBot started.");

	if (graph == nullptr && device)
		graph = new Graphics();

	if (afkTimer.elapsedTime() > 150)
	{
		afkTimer.start();
		*(int*)0x00B499A4 = PerformanceCount();
	}

	graph->Pulse(device);

	if (me && ((Unit*)me)->Class() == CLASS_PALADIN)
		PaladinLeveling();

#pragma region TestRegion

	// here is an area to random tests
	if (do_something)
	{
		++silly_counter;

		auto sp = Spell("Blessing of Might");

		sp.Cast((Unit*)me);

		/*if (me)
		{
			*(int*)(me->addr + 0x1A64) = silly_counter;

			me->UpdateDisplayInfo();
		}*/
		
		do_something = false;
	}

	//mrint("model", std::to_string(silly_counter));	
#pragma endregion

	frameCount++;

	auto det = g_Detours["EndScene"];
	det->Restore();
	int res = ((int(__stdcall*)(LPDIRECT3DDEVICE9))det->GetOrig())(device);
	if (shouldRemoveEndSceneInjection)
	{
		chat("Removing EndScene injection");
		auto it = g_Detours.find("EndScene");
		delete it->second;
		g_Detours.erase(it);

		if (graph != nullptr)
			delete graph;

		endSceneUnhooked = true;
	}
	else
	{
		det->Apply();
	}
	return res;
}

int __stdcall ResetDetour(LPDIRECT3DDEVICE9 device, int pp)
{
	printf("\nResetDetour\n");
	chat("ResetDetour");

	if (graph != nullptr)
		graph->ReleaseFont();

	auto det = g_Detours["Reset"];
	det->Restore();
	int res = ((int(__stdcall*)(LPDIRECT3DDEVICE9, int))det->GetOrig())(device, pp);
	det->Apply();
	return res;
}