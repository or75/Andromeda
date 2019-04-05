#include "Hook.hpp"

#include "../Client.hpp"

#include "../Engine.hpp"
#include "../Engine/Render.hpp"

#include "../feature/Gui.hpp"

namespace source
{
	namespace hook
	{
		Reset_t Reset_o;

		auto WINAPI Hook_Reset( LPDIRECT3DDEVICE9 pDevice , D3DPRESENT_PARAMETERS* pPresentationParameters )->HRESULT
		{
			auto& render = engine::Render::Instance();
			auto& gui = feature::Gui::Instance();

			render.DeviceLost();
			gui.DeviceLost();

			auto res = Reset_o( pDevice , pPresentationParameters );

			m_engine_client->GetScreenSize( iScreenWidth , iScreenHeight );

			render.DeviceReset();
			gui.DeviceReset();

			return res;
		}
	}
}