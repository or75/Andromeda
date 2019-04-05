#include "Hook.hpp"

#include "../Client.hpp"

namespace source
{
	namespace hook
	{
		Present_t Present_o;

		auto WINAPI Hook_Present( LPDIRECT3DDEVICE9 pDevice , CONST RECT* pSourceRect , CONST RECT* pDestRect , HWND hDestWindowOverride , CONST RGNDATA* pDirtyRegion )->HRESULT
		{
			OnRender();

			return Present_o( pDevice , pSourceRect , pDestRect , hDestWindowOverride , pDirtyRegion );
		}
	}
}