#pragma once

#include "../Engine.hpp"

#include "Font.hpp"

#undef CreateFont

namespace source
{
	enum FontRenderFlag_t
	{
		FONT_RENDER_RIGHT = 0 ,
		FONT_RENDER_LEFT = 1 ,
		FONT_RENDER_CENTER_V = 2 ,
		FONT_RENDER_CENTER_H = 4 ,
		FONT_RENDER_CENTER = FONT_RENDER_CENTER_V | FONT_RENDER_CENTER_H
	};

	namespace engine
	{
		typedef struct D3DTLVERTEX
		{
			float x;
			float y;
			float z;
			float rhw;
			D3DCOLOR dxColor;
		} *PD3DTLVERTEX;

		class Render : public Singleton< Render >
		{
		public:
			auto Create( IDirect3DDevice9* direct_device ) -> bool;
			auto Destroy() -> void;

			auto CreateFont( const string& font_name , int font_size , DWORD font_flags = FONT_CREATE_NONE ) -> hfont;

			auto Begin() -> void;
			auto End() -> void;

			auto DeviceLost() -> void;
			auto DeviceReset() -> void;

			auto Line( int x1 , int y1 , int x2 , int y2 , Color color ) -> void;
			auto Line( int x1 , int y1 , Vector2 xy2 , Color color ) -> void;
			auto Line( Vector2 xy1 , Vector2 xy2 , Color color ) -> void;

			auto Box( int x , int y , int w , int h , Color color ) -> void;
			auto Box( int x , int y , Vector2 wh , Color color ) -> void;
			auto Box( Vector2 xy , Vector2 wh , Color color ) -> void;

			auto FillBox( int x , int y , int w , int h , Color color ) -> void;
			auto FillBox( int x , int y , Vector2 wh , Color color ) -> void;
			auto FillBox( Vector2 xy , Vector2 wh , Color color ) -> void;
			
			auto Circle( int x , int y , int r , Color color ) -> void;
			auto Circle( Vector2 xy , int r , Color color ) -> void;

			auto OutlineBox( int x , int y , int w , int h , Color color ) -> void;
			auto OutlineBox( int x , int y , Vector2 wh , Color color ) -> void;
			auto OutlineBox( Vector2 xy , Vector2 wh , Color color ) -> void;
			
			auto Text( hfont font_index , int x , int y , DWORD font_align , Color color , const char* text , ... ) -> void;

			auto GetTextSize( hfont font_index , int& w , int& h , const char* text , ... ) -> void;

		private:
			auto SetVertextState() -> void;
			auto CreateVertex( int x , int y , DWORD color , PD3DTLVERTEX vertext , DWORD index ) -> void;
			auto CreateVertex( float x , float y , DWORD color , PD3DTLVERTEX vertext , DWORD index ) -> void;

			IDirect3DDevice9*				m_direct_device;
			vector<Font*>					m_font_list;
			IDirect3DVertexDeclaration9*	m_direct_vertex_decl;
			IDirect3DVertexShader9*			m_direct_vertex_shader;
			IDirect3DStateBlock9*			m_pStateBlock;

			DWORD							dwOld_D3DRS_COLORWRITEENABLE;
			DWORD							dwOld_D3DRS_MULTISAMPLEANTIALIAS;
			DWORD							dwOld_D3DRS_ANTIALIASEDLINEENABLE;
		};
	}
}