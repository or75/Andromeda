#pragma once

#include "../Include/Auto.hpp"

namespace Andromeda
{
	using pLoadLibraryA = decltype( &LoadLibraryA );
	using pGetModuleHandleA = decltype( &GetModuleHandleA );
	using pGetProcAddress = decltype( &GetProcAddress );

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

	struct loader_data_s
	{
		char m_cheat_dir[MAX_PATH];
	};

	class ImageLoader : public Singleton< ImageLoader >
	{
	public:
		auto OnLoadImage( HMODULE DllImage , PVOID pReserved ) -> bool;
		auto Destroy() -> void;

	public:
		auto GetDllDir() -> string;
		auto GetGameDir() -> string;

	private:
		string m_dll_dir = "";
		string m_game_dir = "";
	};
}