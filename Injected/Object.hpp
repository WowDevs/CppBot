#pragma once
#include "Wowtypes.h"

class LocalPlayer;
extern LocalPlayer* me;

class Object
{
public:
	int addr;
	Location pos;

	Object() : addr{ 0 }, pos{ Location() } { }

	Object(int address) : addr{ address }, pos{ Location() } { }

	~Object() { }

	int Descriptors() { return addr ? *(int*)(addr + 8) : 0; }
		
	template<typename DSC>
	DSC GetDescriptor(int offset)
	{
		int desc = Descriptors();
		return desc ? *(DSC*)(desc + offset * 4) : 0;
	}

	int GetVirtualFuncAddr(int offset) { return addr ? *(int*)(*(int*)addr + 4 * offset) : 0; }

	uint64 Guid() { return GetDescriptor<uint64>(OBJECT_FIELD_GUID); }

	char* Name() { return  addr ? ((char*(__thiscall*)(int))(GetVirtualFuncAddr(54)))(addr) : ""; }

	Location Coords()
	{
		if (addr)
			((void(__thiscall*)(int, Location*))(GetVirtualFuncAddr(12)))(addr, &pos);

		return pos;
	}

	bool HasZeroCoords()
	{
		return Coords().Invalid();
	}

	float Rotation()
	{
		return addr ? ((float(__thiscall*)(int))(GetVirtualFuncAddr(14)))(addr) : 0;
	}

	float DistanceTo(Location& to)
	{
		return Coords().DistanceTo(to);
	}

	char Traceline(Location& start, Location& end, Location& result, int flags)
	{
		float dist = 1.0f;
		return ((char(__cdecl*)(Location&, Location&, Location&, float&, int, int))(0x007A3B70))(start, end, result, dist, flags, 0);
	}

	bool InLosTo(Location& other)
	{
		if (!addr)
			return false;
		if (other.Invalid())
			return false;
		Location start = Coords();
		if (start.Invalid())
			return false;
		Location result;
		start.z += 1.3f;
		other.z += 1.3f;
		return Traceline(start, other, result, 0x100011) == 0;     //0x120171  0x100121  
	}	

	bool InLos()
	{
		/*Location other = ((Object*)GetLocalPlayer())->Coords();*/
		auto ome = ((Object*)me);
		return InLosTo(ome ? ome->Coords() : Location());
	}

	int Type() { return GetDescriptor<int>(OBJECT_FIELD_TYPE); }
	bool IsUnit() { return (Type() & TYPEMASK_UNIT) != 0; }
	bool IsPlayer() { return (Type() & TYPEMASK_PLAYER) != 0; }
	bool IsItem() { return (Type() & TYPEMASK_ITEM) != 0; }

	bool HasFlag(int idx, int flag)
	{
		int flags = GetDescriptor<int>(idx);
		return (flags & flag) != 0;
	}	
};























//int GetLocalPlayer()
//{
//	long long lpguid = addr && ((long long(__cdecl*)())(0x004D3790))();	//ClntObjMgrGetActivePlayerGuid()
//	return lpguid && ((int(__cdecl*)(long long, int))0x004D4DB0)(lpguid, TYPEMASK_PLAYER);
//}

/*byte* GetDescriptorByteP(int offset) { return Descriptors() ? (byte*)(Descriptors() + offset * 4) : nullptr; }
int GetDescriptorInt(int offset) { return Descriptors() ? *(int*)(Descriptors() + offset * 4) : 0; }
float GetDescriptorFloat(int offset) { return Descriptors() ? *(float*)(Descriptors() + offset * 4) : 0; }*/