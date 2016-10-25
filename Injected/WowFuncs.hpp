#pragma once

int PerformanceCount() { return ((int(__cdecl*)())0x0086AE20)(); }

//void WowClient_GetRow(int& row) { return ((void(__thiscall*)(int&))0x008B7DA0)(row); }

template <typename ROW>
bool WowClient_GetLocalizedRow(int table, int index, ROW& row)
{
	return ((bool(__thiscall*)(int, int, ROW&))0x004CFD20)(table, index, row);
}