#pragma once

#include "../Include/Auto.hpp"

namespace Andromeda
{
	struct loader_data_s
	{
		char m_cheat_dir[MAX_PATH];
		DWORD m_cheat_key;
	};

	class ImageLoader : public Singleton< ImageLoader >
	{
	public:
		auto OnLoadImage( HMODULE DllImage , PVOID pReserved ) -> bool;
		auto Destroy() -> void;
		auto GetDllDir() -> string;

	private:
		loader_data_s m_data = { };
		string m_dll_dir = "";
	};
}