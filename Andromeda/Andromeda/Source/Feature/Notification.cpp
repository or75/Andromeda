#include "Notification.hpp"
#include "Gui.hpp"

#include "../Engine/Script.hpp"

namespace source
{
	namespace feature
	{
		auto Notification::RenderNotification() -> void
		{
			if ( m_notification.empty() )
				return;

			for ( size_t Index = 0; Index < m_notification.size(); Index++ )
			{
				auto& notifi = m_notification[Index];

				float end_time = static_cast<float>( notifi.m_show_time );
				float cur_time = static_cast<float>( GetTickCount() );
				float left_sec = ( end_time - cur_time ) / 1000.f;

				if ( left_sec > 0.f && notifi.m_draw )
				{
					auto push_style = false;
					auto padding = 10.f;

					string buff_unicode = notifi.m_msg;

					auto notification_width = ImGui::CalcTextSize( buff_unicode.c_str() ).x + padding + 10.f;
					auto notification_height = 70.f;

					auto screen_w = static_cast<float>( iScreenWidth );
					auto screen_h = static_cast<float>( iScreenHeight );

					auto notification_size = ImVec2( notification_width , notification_height );
					auto notification_pos = ImVec2( screen_w - padding - notification_size.x , screen_h - ( ( notification_size.y + padding ) * ( Index + 1 ) ) );

					ImGui::SetNextWindowSize( notification_size , ImGuiCond_Always );
					ImGui::SetNextWindowPos( notification_pos , ImGuiCond_Always );

					string window_name_id = "";
					ImColor notifi_color = ImColor( 0 , 160 , 255 );

					if ( notifi.m_type == nt_error )
					{
						window_name_id = XorStr( "Error##" );
						notifi_color = ImColor( 255 , 0 , 0 );
					}
					else if ( notifi.m_type == nt_warning )
					{
						window_name_id = XorStr( "Warning##" );
						notifi_color = ImColor( 255 , 160 , 0 );
					}
					else if ( notifi.m_type == nt_info )
					{
						window_name_id = XorStr( "Info##" );
						notifi_color = ImColor( 0 , 160 , 255 );
					}
					else if ( notifi.m_type == nt_success )
					{
						window_name_id = XorStr( "Success##" );
						notifi_color = ImColor( 0 , 200 , 0 );
					}

					window_name_id += to_string( Index );

					if ( left_sec <= 2.f )
					{
						ImGui::PushStyleVar( ImGuiStyleVar_Alpha , 1.5f - ( 1.f / left_sec ) );
						push_style = true;
					}

					ImGui::PushStyleColor( ImGuiCol_TitleBg , notifi_color.operator ImVec4() );
					ImGui::PushStyleColor( ImGuiCol_TitleBgActive , notifi_color.operator ImVec4() );

					if ( ImGui::Begin( window_name_id.c_str() , &notifi.m_draw , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing ) )
					{
						if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_RootWindow ) )
							notifi.m_show_time = GetTickCount() + ( notifi.m_show_sec * 1000 );

						ImGui::TextColored( notifi_color , buff_unicode.c_str() );
						ImGui::End();
					}

					ImGui::PopStyleColor();
					ImGui::PopStyleColor();

					if ( push_style )
						ImGui::PopStyleVar();
				}
				else
				{
					m_notification.erase( m_notification.begin() + Index );
				}
			}
		}

		auto Notification::AddNotification( DWORD show_sec , notify_type type , const char* format , ... ) -> void
		{
			char buf[2048] = { 0 };

			va_list list;
			va_start( list , format );
			vsnprintf_s( buf , sizeof( buf ) - 1 , _TRUNCATE , format , list );
			va_end( list );

			if ( type < nt_error || type > nt_success )
				type = nt_info;

			string message = buf;

			m_notification.push_back( { type , message , GetTickCount() + ( show_sec * 1000 ) , show_sec } );

			m_surface->PlaySoundFile( XorStr( "\\andromeda\\notification.mp3" ) );
		}
	}
}