#pragma once
#include <string>
#include <time.h>
#include <algorithm>
#include "Wowtypes.h"

void FramescriptExecute(const char* text)
{
	((void(__cdecl*)(const char*, const char*, void*))0x00819210)(text, "CppBot", nullptr);
}

void chat(std::string message)
{	
	long long lpguid = ((long long(__cdecl*)())(0x004D3790))();	//ClntObjMgrGetActivePlayerGuid()
	if (!(lpguid && ((int(__cdecl*)(long long, int))0x004D4DB0)(lpguid, TYPEMASK_PLAYER)))
		return;

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "[%H:%M:%S] ", timeinfo);	
	
	std::string taggedMessage = buffer + message;
	std::replace(taggedMessage.begin(), taggedMessage.end(), '\'', '_');
	std::string to_print = "print ('" + taggedMessage + "')";
	FramescriptExecute(to_print.c_str());
}