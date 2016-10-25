#pragma once
#include <vector>

#include "LocalPlayer.hpp"
#include "Container.hpp"
#include "GameObject.hpp"
#include "Corpse.hpp"
#include "DynamicObject.hpp"

std::vector<GameObject*> gameobjects;
std::vector<Item*> items;
std::vector<Unit*> units;
std::vector<Player*> players;
std::map<long long, Object*> objects;

LocalPlayer* me = nullptr;
Unit* target = nullptr;
Unit* focus = nullptr;

long long playerGuid = 0;
long long targetGuid = 0;
long long focusGuid = 0;

inline int ClientConnection()
{
	return *(int*)0x00C79CE0;    //      StaticClientConnection                   
}

inline int ObjManager()
{
	return ClientConnection()? *(int*)(ClientConnection() + 0x2ED0) : 0;
}

inline long long GetPlayerGuid()
{
	return  ObjManager()? *(long long*)(ObjManager() + 0xC0) : 0;	//((long long(__cdecl*)())0x004D3790)(); 	
}

inline int GetObjectByGuidCheck(long long guid, int typemask = -1)
{
	return  guid? ((int(__cdecl*)(long long, int))0x004D4DB0)(guid, typemask) : 0;
}

inline int GetPlayerAddr()
{
	return GetObjectByGuidCheck(GetPlayerGuid(), TYPEMASK_PLAYER);
}

inline long long GetTargetGuid()
{
	return *(long long*)0x00BD07B0;
}

inline long long GetFocusGuid()
{
	return *(long long*)0x00BD07D0;
}

inline Object* GetObjectByGuid(long long guid)
{	
	return objects[guid];
}

// -------------  P u l s e --------------
int __cdecl EnumVisibleObjectsCallback(long long guid, int filter)
{
	int pointer = GetObjectByGuidCheck(guid, -1);
	if (pointer == 0)
		return 1;

	auto type = *(int*)(*(int*)(pointer + 8) + 8);       //obj.Type;

	if (type & TYPEMASK_PLAYER)
	{
		if (guid == playerGuid)
		{
			me = new LocalPlayer(pointer);	
			objects[guid] = me;
			if (guid == targetGuid)
			{
				target = me;				
			}
			if (guid == focusGuid)
			{
				focus = me;
			}
		}
		else
		{
			auto pl = new Player(pointer);
			units.push_back(pl);
			players.push_back(pl);
			objects[guid] = pl;
			if (guid == targetGuid)
			{
				target = pl;
			}
			if (guid == focusGuid)
			{
				focus = pl;
			}
		}
	}
	else if (type & TYPEMASK_UNIT)
	{
		auto uni = new Unit(pointer);
		objects[guid] = uni;
		if (uni->GetCreatureType() != CREATURE_TYPE_CRITTER)
		{
			units.push_back(uni);
		}
		if (guid == targetGuid)
		{
			target = uni;
		}
		if (guid == focusGuid)
		{
			focus = uni;
		}
	}
	else if (type & TYPEMASK_CONTAINER)
	{
		auto con = new Container(pointer);
		items.push_back(con);
		objects[guid] = con;
	}
	else if (type & TYPEMASK_ITEM)
	{
		auto ite = new Item(pointer);
		items.push_back(ite);
		objects[guid] = ite;
	}	
	else if (type & TYPEMASK_GAMEOBJECT)
	{
		auto go = new GameObject(pointer);
		gameobjects.push_back(go);
		objects[guid] = go;
	}
	else if (type & TYPEMASK_CORPSE)
	{
		auto cor = new WoWCorpse(pointer);
		objects[guid] = cor;
	}
	else if (type & TYPEMASK_DYNAMICOBJECT)
	{
		auto dyno = new DynamicObject(pointer);
		objects[guid] = dyno;
	}
	else
	{
		auto obj = new Object(pointer);
		objects[guid] = obj;
	}
	
	return 1;
}

void mrint(std::string /*name*/, std::string /*value*/);

void OM_Pulse()
{
	for (auto& obj : objects)
		delete obj.second;	

	me = nullptr;
	target = nullptr;
	focus = nullptr;

	gameobjects.clear();
	items.clear();
	units.clear();
	players.clear();
	objects.clear();
	
	if(GetPlayerAddr() == 0) 
		return;

	playerGuid = GetPlayerGuid();
	targetGuid = GetTargetGuid();
	focusGuid = GetFocusGuid();

	((int(__cdecl*)(int, int))0x004D4B30)((int)EnumVisibleObjectsCallback, 0);

	mrint("players", std::to_string(players.size()));
	mrint("units", std::to_string(units.size()));
	mrint("items", std::to_string(items.size()));
	mrint("gameobjects", std::to_string(gameobjects.size()));
	mrint("objects", std::to_string(objects.size()));
}