#pragma once

#include "Main.h"

#define DEBUG_INJECT 0

using pLoadLibraryA = decltype( &LoadLibraryA );
using pGetModuleHandleA = decltype( &GetModuleHandleA );
using pGetProcAddress = decltype( &GetProcAddress );
using PDLL_MAIN = bool( WINAPI * )( HMODULE , DWORD , PVOID );

typedef struct
{
	WORD offset : 12;
	WORD type : 4;
} IMAGE_RELOC , *PIMAGE_RELOC;

typedef struct _MANUAL_INJECT
{
	PVOID ImageBase;
	PVOID ShellCode;
	PIMAGE_NT_HEADERS NtHeaders;
	PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;
	pLoadLibraryA fnLoadLibraryA;
	pGetModuleHandleA fnGetModuleHandleA;
	pGetProcAddress fnGetProcAddress;
	char m_cheat_dir[MAX_PATH] = { 0 };
	DWORD m_cheat_key = 0;
}MANUAL_INJECT , *PMANUAL_INJECT;

namespace Inject
{
	bool GetDllInject();
	void InjectCSGO();
}