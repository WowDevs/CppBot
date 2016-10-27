#pragma once
#include<string>
#include "Wowtypes.h"
#include "WowFuncs.hpp"
#include "LocalPlayer.hpp"

//class Unit;
//class LocalPlayer;
////extern LocalPlayer* me;

int SpellIdByName(char*);



class Spell
{
public:
	int id;
	SpellRec record;	

	Spell(char* arg_name)
	{		
		id = SpellIdByName(arg_name);
		WowClient_GetLocalizedRow(0x00AD49D0, id, record);
	}

	bool InRange(Unit* u)
	{
		if (!u || u->HasZeroCoords() || !me)
			return false;		
		
		int mea = ((Object*)me)->addr;
		if (u->addr == mea)
			return true;

		bool inRange = false;

		bool validUnit = ((bool(__cdecl*)(int, SpellRec&, uint64, bool&, int, int))(0x00809610))(mea, record, u->Guid(), inRange, 0, 0);
		return validUnit && inRange;
	}

	char* Name() { return record.m_name_lang; }

	bool IsReady() { return CoolDown() <= 0; }

	float CoolDown()
	{
		int start, duration, unk0;
		start = duration = unk0 = 0;
		bool isReady = false;

		((bool(__cdecl*)(int, bool, int&, int&, bool&, int&))(0x00809000))(id, /*isPet*/ false, duration, start, isReady, unk0);

		int result = start + duration - PerformanceCount();
		return isReady ? (result > 0 ? result / 1000.0f : 0) : FLT_MAX;
	}
	
};

int SpellIdByName(char* name)
{
	int rank;
	return ((int(__cdecl*)(char*, int*))(0x00540200))(name, &rank);
}