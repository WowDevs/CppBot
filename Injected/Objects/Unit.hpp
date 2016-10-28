#pragma once
#include "Object.hpp"

Object* GetObjectByGuid(uint64 /* guid*/);

class Unit : public Object
{
public:
	Unit(int pointer) : Object(pointer) { }

	CreatureType GetCreatureType() { return  addr ? ((CreatureType(__thiscall*)(int))(0x0071F300))(addr) : CREATURE_TYPE_NOT_SPECIFIED; }

	uint64 TargetGuid()
	{
		return GetDescriptor<uint64>(UNIT_FIELD_TARGET);
	}
	
	Unit* Target()
	{	
		return (Unit*)GetObjectByGuid(TargetGuid());
	}

	int Health()
	{
		return GetDescriptor<int>(UNIT_FIELD_HEALTH);
	}

	int MaxHealth()
	{
		return GetDescriptor<int>(UNIT_FIELD_MAXHEALTH);
	}

	double HealthPercent()
	{
		return (Health() / (double)MaxHealth()) * 100;
	}

	bool IsDead()
	{
		return Health() <= 0 || HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
	}

	uint64 SummonedBy()
	{
		return GetDescriptor<uint64>(UNIT_FIELD_SUMMONEDBY);
	}

	bool FeignDeathed()
	{
		return HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
	}

	byte* DisplayPower()
	{
		return GetDescriptor<byte*>(UNIT_FIELD_POWER1);
	}

	byte* UnitBytes0()
	{
		return GetDescriptor<byte*>(UNIT_FIELD_BYTES_0);
	}

	byte* UnitBytes1()
	{
		GetDescriptor<byte*>(UNIT_FIELD_BYTES_1);
	}

	byte* UnitBytes2()
	{
		return GetDescriptor<byte*>(UNIT_FIELD_BYTES_2);
	}

	int Race()
	{
		return UnitBytes0()[0];
	}

	int Class()
	{
		return UnitBytes0()[1];
	}

	int Gender()
	{
		return UnitBytes0()[2];
	}

	int PowerType()
	{
		return UnitBytes0()[3];
	}

	bool IsLootable()
	{
		HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
	}

	bool IsTapped()
	{
		return HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TAPPED);
	}

	bool IsTappedByMe()
	{
		return HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TAPPED_BY_PLAYER);
	}

	bool IsInCombat()
	{
		return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
	}

	bool IsLooting()
	{
		return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING); 
	}

	int GetUInt32Value(int index)
	{
		return GetDescriptor<int>(index);
	}

	bool CanAttack(Unit* other)
	{
		if (!other || other->addr == 0)
			return false;

		if (!other->IsPlayer() && other->IsDead()) 
			return false;

		return  addr ? ((bool(__thiscall*)(int, int))(0x00729740))(addr, other->addr) : false;
	}

	bool CanAttack()
	{
		return CanAttack((Unit*)me);
	}
};