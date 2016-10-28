#pragma once
#include "Player.hpp"

class LocalPlayer : public Player
{
public:
	LocalPlayer(int pointer) : Player(pointer) { }

	uint64 PetGuid()
	{
		if (addr && *(int*)(addr + 0x1858) & 1)
		{
			int petGuidPonter = *(int*)(addr + 0x1008);
			return *(uint64*)(petGuidPonter + 0x770);
		}
		return 0;
	}

	Unit* Pet() { return (Unit*)GetObjectByGuid(PetGuid()); }
};