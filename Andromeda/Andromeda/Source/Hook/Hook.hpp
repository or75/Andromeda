#pragma once

#include "../../Valve/SDK.hpp"

namespace source
{
	namespace hook
	{
		using Present_t = HRESULT( WINAPI* )( LPDIRECT3DDEVICE9 pDevice , CONST RECT* pSourceRect , CONST RECT* pDestRect , HWND hDestWindowOverride , CONST RGNDATA* pDirtyRegion );
		using Reset_t = HRESULT( WINAPI* )( LPDIRECT3DDEVICE9 pDevice , D3DPRESENT_PARAMETERS* pPresentationParameters );

		extern Present_t Present_o;
		extern Reset_t Reset_o;

		auto WINAPI Hook_Present( LPDIRECT3DDEVICE9 pDevice , CONST RECT* pSourceRect , CONST RECT* pDestRect , HWND hDestWindowOverride , CONST RGNDATA* pDirtyRegion ) -> HRESULT;
		auto WINAPI Hook_Reset( LPDIRECT3DDEVICE9 pDevice , D3DPRESENT_PARAMETERS* pPresentationParameters ) -> HRESULT;
		auto WINAPI Hook_CreateMove( float flInputSampleTime , CUserCmd* cmd ) -> bool;
		auto WINAPI Hook_LockCursor() -> void;
	}
}