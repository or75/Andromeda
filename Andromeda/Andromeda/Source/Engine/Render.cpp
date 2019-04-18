#include "Render.hpp"

#define D3DFVF_CUSTOM_TEXT ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define D3DFVF_CUSTOM_VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define CIRCLE_RESOLUTION 64

namespace source
{
	namespace engine
	{
		auto Render::Create( IDirect3DDevice9* direct_device ) -> bool
		{
			m_direct_device = direct_device;

			return true;
		}

		auto Render::Destroy() -> void
		{
			m_direct_device = nullptr;

			if ( !m_font_list.empty() )
			{
				for ( auto& font : m_font_list )
					font->Destroy();
			}
		}

		auto Render::CreateFont( const string & font_name , int font_size , DWORD font_flags ) -> hfont
		{
			auto p_font = new Font();

			p_font->Create( m_direct_device , font_name , font_size , font_flags );

			m_font_list.push_back( p_font );

			return ( m_font_list.size() - 1 );
		}

		auto Render::Begin() -> void
		{
			if ( m_direct_device->CreateStateBlock( D3DSBT_PIXELSTATE , &m_pStateBlock ) == D3D_OK )
			{
				m_pStateBlock->Capture();

				m_direct_device->GetRenderState( D3DRS_COLORWRITEENABLE , &dwOld_D3DRS_COLORWRITEENABLE );
				m_direct_device->GetRenderState( D3DRS_MULTISAMPLEANTIALIAS , &dwOld_D3DRS_MULTISAMPLEANTIALIAS );
				m_direct_device->GetRenderState( D3DRS_ANTIALIASEDLINEENABLE , &dwOld_D3DRS_ANTIALIASEDLINEENABLE );

				m_direct_device->GetVertexDeclaration( &m_direct_vertex_decl );
				m_direct_device->GetVertexShader( &m_direct_vertex_shader );

				m_direct_device->SetPixelShader( NULL );
				m_direct_device->SetVertexShader( NULL );

				m_direct_device->SetRenderState( D3DRS_COLORWRITEENABLE , 0xffffffff );
				m_direct_device->SetRenderState( D3DRS_SRGBWRITEENABLE , false );
				m_direct_device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS , false );
				m_direct_device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE , false );

				SetVertextState();

				m_direct_device->SetSamplerState( NULL , D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP );
				m_direct_device->SetSamplerState( NULL , D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP );
				m_direct_device->SetSamplerState( NULL , D3DSAMP_ADDRESSW , D3DTADDRESS_WRAP );
				m_direct_device->SetSamplerState( NULL , D3DSAMP_SRGBTEXTURE , NULL );
			}
		}

		auto Render::End() -> void
		{
			if ( m_pStateBlock )
			{
				m_direct_device->SetRenderState( D3DRS_COLORWRITEENABLE , dwOld_D3DRS_COLORWRITEENABLE );
				m_direct_device->SetRenderState( D3DRS_SRGBWRITEENABLE , true );
				m_direct_device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS , dwOld_D3DRS_MULTISAMPLEANTIALIAS );
				m_direct_device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE , dwOld_D3DRS_ANTIALIASEDLINEENABLE );

				m_direct_device->SetVertexDeclaration( m_direct_vertex_decl );
				m_direct_device->SetVertexShader( m_direct_vertex_shader );

				m_pStateBlock->Apply();
				m_pStateBlock->Release();
				m_pStateBlock = nullptr;
			}
		}

		auto Render::DeviceLost() -> void
		{
			if ( !m_font_list.empty() )
			{
				for ( auto& font : m_font_list )
				{
					if ( font )
						font->OnLost();
				}
			}
		}

		auto Render::DeviceReset() -> void
		{
			if ( !m_font_list.empty() )
			{
				for ( auto& font : m_font_list )
				{
					if ( font )
						font->OnReset();
				}
			}
		}

		auto Render::Line( int x1 , int y1 , int x2 , int y2 , Color color ) -> void
		{
			auto dxLineColor = color.Code();

			D3DTLVERTEX D3DTLV[2] = { 0 };

			CreateVertex( x1 , y1 , dxLineColor , PD3DTLVERTEX( &D3DTLV ) , 0 );
			CreateVertex( x2 , y2 , dxLineColor , PD3DTLVERTEX( &D3DTLV ) , 1 );

			m_direct_device->SetFVF( D3DFVF_CUSTOM_VERTEX );

			m_direct_device->DrawPrimitiveUP( D3DPT_LINELIST , 1 , &D3DTLV , sizeof( D3DTLVERTEX ) );
		}

		auto Render::Line( int x1 , int y1 , Vector2 xy2 , Color color ) -> void
		{
			int x2 = static_cast<int>( xy2.m_x );
			int y2 = static_cast<int>( xy2.m_y );

			Line( x1 , y1 , x2 , y2 , color );
		}

		auto Render::Line( Vector2 xy1 , Vector2 xy2 , Color color ) -> void
		{
			auto x1 = static_cast<int>( xy1.m_x );
			auto y1 = static_cast<int>( xy1.m_y );
			auto x2 = static_cast<int>( xy2.m_x );
			auto y2 = static_cast<int>( xy2.m_y );

			Line( x1 , y1 , x2 , y2 , color );
		}

		auto Render::Box( int x , int y , int w , int h , Color color ) -> void
		{
			auto dxBoxColor = color.Code();

			D3DTLVERTEX D3DTLV[8] = { 0 };

			// top
			CreateVertex( x , y , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 0 );
			CreateVertex( x + w , y , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 1 );

			// left
			CreateVertex( x , y , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 2 );
			CreateVertex( x , y + h , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 3 );

			// right
			CreateVertex( x + w - 1 , y , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 4 );
			CreateVertex( x + w - 1 , y + h , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 5 );

			// bottom
			CreateVertex( x , y + h - 1 , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 6 );
			CreateVertex( x + w , y + h - 1 , dxBoxColor , PD3DTLVERTEX( &D3DTLV ) , 7 );

			m_direct_device->SetFVF( D3DFVF_CUSTOM_VERTEX );

			m_direct_device->DrawPrimitiveUP( D3DPT_LINELIST , 4 , &D3DTLV , sizeof( D3DTLVERTEX ) );
		}

		auto Render::Box( int x , int y , Vector2 wh , Color color ) -> void
		{
			auto w = static_cast<int>( wh.m_x );
			auto h = static_cast<int>( wh.m_y );

			Box( x , y , w , h , color );
		}

		auto Render::Box( Vector2 xy , Vector2 wh , Color color ) -> void
		{
			auto x = static_cast<int>( xy.m_x );
			auto y = static_cast<int>( xy.m_y );
			auto w = static_cast<int>( wh.m_x );
			auto h = static_cast<int>( wh.m_y );

			Box( x , y , w , h , color );
		}

		auto Render::FillBox( int x , int y , int w , int h , Color color ) -> void
		{
			auto dxFillBoxColor = color.Code();

			D3DTLVERTEX D3DTLV[4] = { 0 };

			CreateVertex( x , y , dxFillBoxColor , PD3DTLVERTEX( &D3DTLV ) , 0 );
			CreateVertex( x + w , y , dxFillBoxColor , PD3DTLVERTEX( &D3DTLV ) , 1 );
			CreateVertex( x , y + h , dxFillBoxColor , PD3DTLVERTEX( &D3DTLV ) , 2 );
			CreateVertex( x + w , y + h , dxFillBoxColor , PD3DTLVERTEX( &D3DTLV ) , 3 );

			m_direct_device->SetFVF( D3DFVF_CUSTOM_VERTEX );
			m_direct_device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 , &D3DTLV , sizeof( D3DTLVERTEX ) );
		}

		auto Render::FillBox( int x , int y , Vector2 wh , Color color ) -> void
		{
			auto w = static_cast<int>( wh.m_x );
			auto h = static_cast<int>( wh.m_y );

			FillBox( x , y , w , h , color );
		}

		auto Render::FillBox( Vector2 xy , Vector2 wh , Color color ) -> void
		{
			auto x = static_cast<int>( xy.m_x );
			auto y = static_cast<int>( xy.m_y );
			auto w = static_cast<int>( wh.m_x );
			auto h = static_cast<int>( wh.m_y );

			FillBox( x , y , w , h , color );
		}

		auto Render::Circle( int x , int y , int r , Color color ) -> void
		{
			auto dxCircleColor = color.Code();

			D3DTLVERTEX D3DTLV[CIRCLE_RESOLUTION + 1] = { 0 };

			auto Angle = static_cast<float>( ( 2.0f * D3DX_PI ) / CIRCLE_RESOLUTION );

			for ( int i = 0; i <= CIRCLE_RESOLUTION; i++ )
			{
				auto CurAngle = static_cast<float>( i ) * Angle;
				auto Radius = static_cast<float>( r );

				auto DrawPosX = ( floorf( static_cast<float>( x ) ) + Radius * cos( CurAngle ) );
				auto DrawPosY = ( floorf( static_cast<float>( y ) ) - Radius * sin( CurAngle ) );

				CreateVertex( DrawPosX , DrawPosY , dxCircleColor , PD3DTLVERTEX( &D3DTLV ) , i );
			}

			m_direct_device->SetFVF( D3DFVF_CUSTOM_VERTEX );
			m_direct_device->DrawPrimitiveUP( D3DPT_LINESTRIP , CIRCLE_RESOLUTION , D3DTLV , sizeof( D3DTLVERTEX ) );
		}

		auto Render::Circle( Vector2 xy , int r , Color color ) -> void
		{
			auto x = static_cast<int>( xy.m_x );
			auto y = static_cast<int>( xy.m_y );

			Circle( x , y , r , color );
		}

		auto Render::OutlineBox( int x , int y , int w , int h , Color color ) -> void
		{
			Box( x , y , w , h , Color( 0 , 0 , 0 , 255 ) );
			Box( x + 1 , y + 1 , w - 2 , h - 2 , color );
			Box( x + 2 , y + 2 , w - 4 , h - 4 , Color( 0 , 0 , 0 , 255 ) );
		}

		auto Render::OutlineBox( int x , int y , Vector2 wh , Color color ) -> void
		{
			auto w = static_cast<int>( wh.m_x );
			auto h = static_cast<int>( wh.m_y );

			OutlineBox( x , y , w , h , color );
		}

		auto Render::OutlineBox( Vector2 xy , Vector2 wh , Color color ) -> void
		{
			auto x = static_cast<int>( xy.m_x );
			auto y = static_cast<int>( xy.m_y );
			auto w = static_cast<int>( wh.m_x );
			auto h = static_cast<int>( wh.m_y );

			OutlineBox( x , y , w , h , color );
		}

		auto Render::Text( hfont font_index , int x , int y , DWORD font_align , Color color , const char * text , ... ) -> void
		{
			if ( font_index >= m_font_list.size() || font_index < 0 )
				return;

			auto pFont = m_font_list[font_index];

			if ( !pFont )
				return;

			char buffer[2048] = { 0 };

			va_list args;
			va_start( args , text );
			vsprintf_s( buffer , text , args );
			va_end( args );

			if ( font_align != FONT_RENDER_RIGHT )
			{
				auto w = 0 , h = 0;

				pFont->GetTextSize( buffer , w , h );

				if ( font_align & FONT_RENDER_LEFT )
					x -= w;
				else if ( font_align & FONT_RENDER_CENTER_H )
					x -= w / 2;

				if ( font_align & FONT_RENDER_CENTER_V )
					y -= h / 2;
			}

			m_direct_device->SetFVF( D3DFVF_CUSTOM_TEXT );

			pFont->Text( x , y , color , buffer );
		}

		auto source::engine::Render::GetTextSize( hfont font_index , int& w , int& h , const char* text , ... ) -> void
		{
			if ( font_index >= m_font_list.size() || font_index < 0 )
				return;

			auto pFont = m_font_list[font_index];

			if ( !pFont )
				return;

			char buffer[2048] = { 0 };

			va_list args;
			va_start( args , text );
			vsprintf_s( buffer , text , args );
			va_end( args );

			pFont->GetTextSize( buffer , w , h );
		}

		auto Render::SetVertextState() -> void
		{
			m_direct_device->SetRenderState( D3DRS_LIGHTING , false );
			m_direct_device->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
			m_direct_device->SetRenderState( D3DRS_ALPHATESTENABLE , false );
			m_direct_device->SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );
			m_direct_device->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
			m_direct_device->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
		}

		auto Render::CreateVertex( int x , int y , DWORD color , PD3DTLVERTEX vertext , DWORD index ) -> void
		{
			vertext[index].x = static_cast<float>( x );
			vertext[index].y = static_cast<float>( y );
			vertext[index].z = 0.f;
			vertext[index].rhw = 1.f;
			vertext[index].dxColor = color;
		}

		auto Render::CreateVertex( float x , float y , DWORD color , PD3DTLVERTEX vertext , DWORD index ) -> void
		{
			vertext[index].x = x;
			vertext[index].y = y;
			vertext[index].z = 0.f;
			vertext[index].rhw = 1.f;
			vertext[index].dxColor = color;
		}
	}
}