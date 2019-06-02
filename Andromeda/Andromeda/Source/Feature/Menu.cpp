#include "Menu.hpp"

#include "../Engine/Script.hpp"

#include "../Feature/Gui.hpp"
#include "../Feature/Notification.hpp"

#include "../../Include/ImGui/imgui_internal.h"



namespace source
{
	namespace feature
	{
		auto Menu::Create() -> void
		{
			m_start_width = 462.f;
			m_start_height = 550.f;

			m_select_script_index = 0;

			m_show_about = false;
		}

		auto Menu::RenderStartMenu() -> void
		{
			auto& notify = feature::Notification::Instance();
			auto& script_manager = engine::ScriptManager::Instance();
			auto& script_system = script_manager.m_script_system;
			auto& style = ImGui::GetStyle();
			auto& gui = feature::Gui::Instance();

			auto cheld_size_x = 220.f;

			auto button_size_x = ( cheld_size_x - style.ItemSpacing.x ) / 2.f;
			auto button_size_y = 25.f;

			ImGui::PushFont( gui.m_font_unicode_ms );

			ImGui::SetNextWindowSize( ImVec2( m_start_width , m_start_height ) , ImGuiCond_Always );
			ImGui::SetNextWindowPos( ImVec2( 0.f , (float)iScreenHeight - m_start_height ) , ImGuiSetCond_FirstUseEver );

			if ( ImGui::Begin( XorStr( "Andromeda DLC" ) , 0 , ImGuiWindowFlags_NoResize ) )
			{
				ImGui::BeginGroup();
				ImGui::BeginChild( XorStr( "AndromedaChildLeft" ) , ImVec2( cheld_size_x , ImGui::GetContentRegionAvail().y - button_size_y - style.ItemSpacing.y - 2.f ) , true );

				if ( m_select_script_index > script_system->m_module_list.size() )
					m_select_script_index = script_system->m_module_list.size() - 1;

				for ( size_t Index = 0; Index < script_system->m_module_list.size(); Index++ )
				{
					auto s_module = script_system->m_module_list.at( Index );
					auto script_module = s_module.m_script_module;

					if ( !script_module )
						continue;

					string module_name = script_module->GetName();

					ImColor script_status_color( 255 , 255 , 255 );

					if ( s_module.m_enable )
					{
						module_name += XorStr( " (enabled)" );
						script_status_color = ImColor( 0 , 255 , 0 );
					}
					else
					{
						module_name += XorStr( " (disabled)" );
						script_status_color = ImColor( 255 , 140 , 0 );
					}

					ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign , ImVec2( 0.f , 0.5f ) );
					ImGui::PushStyleColor( ImGuiCol_Text , script_status_color.operator ImVec4() );

					module_name = Andromeda::str_wide_to_str_unicode( module_name );

					if ( ImGui::Button( module_name.c_str() , ImVec2( -1.f , 0.f ) ) )
					{
						m_select_script_index = Index;

						ImGui::OpenPopup( XorStr( "ModuleEdit##ModuleList" ) );
					}

					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
				}

				if ( ImGui::BeginPopup( XorStr( "ModuleEdit##ModuleList" ) , ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize ) )
				{
					auto& module = script_system->m_module_list.at( m_select_script_index );

					if ( ImGui::BeginMenu( "Options" ) )
					{
						ImGui::MenuItem( XorStr( "Enable Script##ModuleList" ) , 0 , &module.m_enable );
						ImGui::EndMenu();
					}

					ImGui::Separator();

					if ( ImGui::MenuItem( XorStr( "Unload Script##ModuleList" ) ) )
					{
						string module_name = Andromeda::str_wide_to_str_unicode( module.m_script_module->GetName() );

						module.Unload();

						script_system->m_module_list.erase( script_system->m_module_list.begin() + m_select_script_index );

						ImGui::CloseCurrentPopup();

						notify.AddNotification( 5 , feature::nt_success , XorStr( "module (%s) was successfully unloaded" ) , module_name.c_str() );
					}

					ImGui::EndPopup();
				}

				ImGui::EndChild();

				if ( ImGui::Button( XorStr( "Refresh list" ) , ImVec2( button_size_x , button_size_y ) ) )
				{
					script_manager.UpdateScriptList( true );

					notify.AddNotification( 5 , feature::nt_success , XorStr( "Modules was successfully loaded" ) );
				}

				ImGui::SameLine();

				if ( ImGui::Button( XorStr( "Unload all" ) , ImVec2( button_size_x , button_size_y ) ) )
				{
					script_system->UnloadAll();

					notify.AddNotification( 5 , feature::nt_success , XorStr( "All modules was successfully unloaded" ) );
				}

				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::BeginChild( XorStr( "AndromedaChildRightTop" ) , ImVec2( cheld_size_x , ImGui::GetContentRegionAvail().y - 2.f ) , true );

				if ( ButtonIcon( ICON_FA_CROSSHAIRS , XorStr( "Aimbot (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Aimbot Menu Clicked !" );
				}

				if ( ButtonIcon( ICON_FA_PLUS_CIRCLE , XorStr( "TriggerBot (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "TriggerBot Menu Clicked !" );
				}

				if ( ButtonIcon( ICON_FA_EYE , XorStr( "Visual (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Visual Menu Clicked !" );
				}

				if ( ButtonIcon( ICON_FA_PUZZLE_PIECE , XorStr( "Misc (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Misc Menu Clicked !" );
				}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if ( ButtonIcon( ICON_FA_USERS_COG , XorStr( "Config Manager (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) , -4.f ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Config Menu Clicked !" );
				}

				if ( ButtonIcon( ICON_FA_PALETTE , XorStr( "Color Manager (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Colors Menu Clicked !" );
				}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if ( ButtonIcon( ICON_FA_COG , XorStr( "Settings##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					m_show_settings = !m_show_settings;
				}

				if ( ButtonIcon( ICON_FA_INFO_CIRCLE , XorStr( "About##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					m_show_about = !m_show_about;
				}

				ImGui::EndChild();
				ImGui::EndGroup();

				ImGui::End();
			}

			ImGui::PopFont();
		}

		auto Menu::RenderSettingsMenu() -> void
		{
			auto& gui = feature::Gui::Instance();

			if ( m_show_settings )
			{
				ImGui::PushFont( gui.m_font_unicode_ms );

				ImGui::SetNextWindowSize( ImVec2( 500.f , 300.f ) , ImGuiCond_FirstUseEver );

				if ( ImGui::Begin( XorStr( "Settings" ) , &m_show_settings , ImGuiWindowFlags_NoCollapse ) )
				{
					if ( ImGui::BeginTabBar( XorStr( "SettingsTabBar" ) , ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyScroll ) )
					{
						if ( ImGui::BeginTabItem( XorStr( "Main" ) , 0 , ImGuiTabItemFlags_NoCloseWithMiddleMouseButton ) )
						{
							CheckBox( XorStr( "Welcome Message" ) , XorStr( "##Settings.Tab.Main.1" ) , &config::settings::main::WelcomeMsg , 180.f );
							SliderInt( XorStr( "Script Timeout" ) , XorStr( "##Settings.Tab.Main.2" ) , &config::settings::main::Timeout , 150 , 5000 , 180.f );
							HotKey( XorStr( "Menu Key" ) , XorStr( "##Settings.Tab.Main.3" ) , &config::settings::main::MenuKey , 180.f );

							ImGui::EndTabItem();
						}

						ImGui::EndTabBar();
					}

					SettingsMenuHandle();

					ImGui::End();
				}

				ImGui::PopFont();
			}
		}

		auto Menu::RenderAboutMenu() -> void
		{
			auto& gui = feature::Gui::Instance();

			if ( m_show_about )
			{
				auto w = 500.f;
				auto h = 210.f;

				ImGui::SetNextWindowSize( ImVec2( w , h ) , ImGuiCond_FirstUseEver );
				ImGui::SetNextWindowPos( ImVec2( ( (float)iScreenWidth / 2.f ) - ( w / 2.f ) , ( (float)iScreenHeight / 2.f ) - ( h / 2.f ) ) , ImGuiCond_FirstUseEver );

				ImGui::PushFont( gui.m_font_unicode_ms );

				if ( ImGui::Begin( XorStr( "About" ) , &m_show_about , ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings ) )
				{
					ImGui::PushFont( gui.m_font_color_tube );
					ImGui::Text( XorStr( "Andromeda" ) );
					ImGui::PopFont();

					ImGui::Text( XorStr( "Author: _or_75" ) );
					ImGui::Text( XorStr( "Build: [%s | %s]" ) , __DATE__ , __TIME__ );
					ImGui::Text( XorStr( "ImGui: [%s]" ) , ImGui::GetVersion() );
					ImGui::Text( XorStr( "Version: %s" ) , XorStr( CHEAT_VERSION ) );
					ImGui::Text( XorStr( "Web: [v-hack.ru]" ) );

					ImGui::End();
				}

				ImGui::PopFont();
			}
		}

		auto Menu::ButtonIcon( const char* icon , const char* text , ImVec2 size , float icon_offset ) -> bool
		{
			auto& gui = feature::Gui::Instance();

			auto old_x_offset = gui.m_font_awesome_icon->DisplayOffset.x;

			gui.m_font_awesome_icon->DisplayOffset.x += icon_offset;

			ImGui::PushFont( gui.m_font_awesome_icon );
			ImGui::Text( XorStr( "%s" ) , icon );
			ImGui::PopFont();

			gui.m_font_awesome_icon->DisplayOffset.x = old_x_offset;

			ImGui::SameLine( 40.f );

			return ImGui::Button( text , size );
		}

		auto Menu::CheckBox( const char* title , const char* str_id , int* current_item , float left_padding ) -> void
		{
			if ( title )
			{
				ImGui::Text( title );

				if ( left_padding <= 0.f )
					ImGui::SameLine( ImGui::CalcTextSize( title ).x + 10.f );
				else
					ImGui::SameLine( left_padding );
			}

			const char* CheckBoxItems[] = { XorStr( "Disable" ) , XorStr( "Enable" ) };

			ImGui::PushItemWidth( -1.f );
			ImGui::Combo( str_id , current_item , CheckBoxItems , IM_ARRAYSIZE( CheckBoxItems ) );
			ImGui::PopItemWidth();
		}

		auto Menu::SliderInt( const char* title , const char* str_id , int* v , int min , int max , float left_padding ) -> void
		{
			if ( title )
			{
				ImGui::Text( title );

				if ( left_padding <= 0.f )
					ImGui::SameLine( ImGui::CalcTextSize( title ).x + 10.f );
				else
					ImGui::SameLine( left_padding );
			}

			ImGui::PushItemWidth( -1.f );
			ImGui::SliderInt( str_id , v , min , max );
			ImGui::PopItemWidth();
		}

		auto Menu::HotKey( const char* title , const char* str_id , int* k , float left_padding ) -> bool
		{
			const char* const KeyNames[] = {
			"Unknown",
			"VK_LBUTTON",
			"VK_RBUTTON",
			"VK_CANCEL",
			"VK_MBUTTON",
			"VK_XBUTTON1",
			"VK_XBUTTON2",
			"Unknown",
			"VK_BACK",
			"VK_TAB",
			"Unknown",
			"Unknown",
			"VK_CLEAR",
			"VK_RETURN",
			"Unknown",
			"Unknown",
			"VK_SHIFT",
			"VK_CONTROL",
			"VK_MENU",
			"VK_PAUSE",
			"VK_CAPITAL",
			"VK_KANA",
			"Unknown",
			"VK_JUNJA",
			"VK_FINAL",
			"VK_KANJI",
			"Unknown",
			"VK_ESCAPE",
			"VK_CONVERT",
			"VK_NONCONVERT",
			"VK_ACCEPT",
			"VK_MODECHANGE",
			"VK_SPACE",
			"VK_PRIOR",
			"VK_NEXT",
			"VK_END",
			"VK_HOME",
			"VK_LEFT",
			"VK_UP",
			"VK_RIGHT",
			"VK_DOWN",
			"VK_SELECT",
			"VK_PRINT",
			"VK_EXECUTE",
			"VK_SNAPSHOT",
			"VK_INSERT",
			"VK_DELETE",
			"VK_HELP",
			"0",
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"9",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"A",
			"B",
			"C",
			"D",
			"E",
			"F",
			"G",
			"H",
			"I",
			"J",
			"K",
			"L",
			"M",
			"N",
			"O",
			"P",
			"Q",
			"R",
			"S",
			"T",
			"U",
			"V",
			"W",
			"X",
			"Y",
			"Z",
			"VK_LWIN",
			"VK_RWIN",
			"VK_APPS",
			"Unknown",
			"VK_SLEEP",
			"VK_NUMPAD0",
			"VK_NUMPAD1",
			"VK_NUMPAD2",
			"VK_NUMPAD3",
			"VK_NUMPAD4",
			"VK_NUMPAD5",
			"VK_NUMPAD6",
			"VK_NUMPAD7",
			"VK_NUMPAD8",
			"VK_NUMPAD9",
			"VK_MULTIPLY",
			"VK_ADD",
			"VK_SEPARATOR",
			"VK_SUBTRACT",
			"VK_DECIMAL",
			"VK_DIVIDE",
			"VK_F1",
			"VK_F2",
			"VK_F3",
			"VK_F4",
			"VK_F5",
			"VK_F6",
			"VK_F7",
			"VK_F8",
			"VK_F9",
			"VK_F10",
			"VK_F11",
			"VK_F12",
			"VK_F13",
			"VK_F14",
			"VK_F15",
			"VK_F16",
			"VK_F17",
			"VK_F18",
			"VK_F19",
			"VK_F20",
			"VK_F21",
			"VK_F22",
			"VK_F23",
			"VK_F24",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"VK_NUMLOCK",
			"VK_SCROLL",
			"VK_OEM_NEC_EQUAL",
			"VK_OEM_FJ_MASSHOU",
			"VK_OEM_FJ_TOUROKU",
			"VK_OEM_FJ_LOYA",
			"VK_OEM_FJ_ROYA",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"Unknown",
			"VK_LSHIFT",
			"VK_RSHIFT",
			"VK_LCONTROL",
			"VK_RCONTROL",
			"VK_LMENU",
			"VK_RMENU"
			};

			if ( title )
			{
				ImGui::Text( title );

				if ( left_padding <= 0.f )
					ImGui::SameLine( ImGui::CalcTextSize( title ).x + 10.f );
				else
					ImGui::SameLine( left_padding );
			}

			ImGuiWindow* window = ImGui::GetCurrentWindow();

			if ( window->SkipItems )
				return false;

			ImGuiContext& g = *GImGui;
			ImGuiIO& io = g.IO;

			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID( str_id );
			const ImVec2 label_size = ImGui::CalcTextSize( str_id , NULL , true );

			ImVec2 size = ImGui::CalcItemSize( ImVec2( ImGui::GetContentRegionAvailWidth() , 0.f ) , ImGui::CalcItemWidth() , label_size.y + style.FramePadding.y*2.0f );

			const ImRect frame_bb( window->DC.CursorPos , window->DC.CursorPos + size );
			const ImRect total_bb( window->DC.CursorPos , frame_bb.Max );

			ImGui::ItemSize( total_bb , style.FramePadding.y );

			if ( !ImGui::ItemAdd( total_bb , id ) )
				return false;

			const bool focus_requested = ImGui::FocusableItemRegister( window , g.ActiveId == id );
			const bool focus_requested_by_code = focus_requested && ( window->DC.FocusCounterAll == g.FocusRequestCurrCounterAll );
			const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

			const bool hovered = ImGui::ItemHoverable( frame_bb , id );

			if ( hovered )
			{
				ImGui::SetHoveredID( id );
				g.MouseCursor = ImGuiMouseCursor_TextInput;
			}

			const bool user_clicked = hovered && io.MouseClicked[0];

			if ( focus_requested || user_clicked )
			{
				if ( g.ActiveId != id )
				{
					memset( io.MouseDown , 0 , sizeof( io.MouseDown ) );
					memset( io.KeysDown , 0 , sizeof( io.KeysDown ) );
					*k = 0;
				}
				ImGui::SetActiveID( id , window );
				ImGui::FocusWindow( window );
			}
			else if ( io.MouseClicked[0] )
			{
				if ( g.ActiveId == id )
					ImGui::ClearActiveID();
			}

			bool value_changed = false;
			int key = *k;

			if ( g.ActiveId == id )
			{
				for ( auto i = 0; i < 5; i++ )
				{
					if ( io.MouseDown[i] )
					{
						switch ( i )
						{
							case 0:
								key = VK_LBUTTON;
								break;
							case 1:
								key = VK_RBUTTON;
								break;
							case 2:
								key = VK_MBUTTON;
								break;
							case 3:
								key = VK_XBUTTON1;
								break;
							case 4:
								key = VK_XBUTTON2;
								break;
						}
						value_changed = true;
						ImGui::ClearActiveID();
					}
				}
				if ( !value_changed )
				{
					for ( auto i = VK_BACK; i <= VK_RMENU; i++ )
					{
						if ( io.KeysDown[i] )
						{
							key = i;
							value_changed = true;
							ImGui::ClearActiveID();
						}
					}
				}

				if ( ImGui::IsKeyPressedMap( ImGuiKey_Escape ) )
				{
					*k = 0;
					ImGui::ClearActiveID();
				}
				else
				{
					*k = key;
				}
			}

			char buf_display[64] = "None";

			ImGui::RenderFrame( frame_bb.Min , frame_bb.Max , ImGui::ColorConvertFloat4ToU32( ImGui::GetStyleColorVec4( ImGuiCol_FrameBg ) ) , true , style.FrameRounding );

			if ( *k != 0 && g.ActiveId != id )
				strcpy_s( buf_display , KeyNames[*k] );
			else if ( g.ActiveId == id )
				strcpy_s( buf_display , "<Press a key>" );

			const ImRect clip_rect( frame_bb.Min.x , frame_bb.Min.y , frame_bb.Min.x + size.x , frame_bb.Min.y + size.y );

			ImGui::RenderTextClipped( frame_bb.Min + style.FramePadding , frame_bb.Max - style.FramePadding , buf_display , NULL , NULL , style.ButtonTextAlign , &clip_rect );

			return value_changed;
		}

		auto Menu::SettingsMenuHandle() -> void
		{
			if ( config::settings::main::Timeout < 150 )
				config::settings::main::Timeout = 150;
			else if ( config::settings::main::Timeout > 5000 )
				config::settings::main::Timeout = 5000;
		}
	}
}