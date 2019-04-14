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
			m_start_height = 560.f;

			m_select_script_index = 0;
		}

		auto Menu::RenderStartMenu() -> void
		{
			auto& notify			= feature::Notification::Instance();
			auto& script_manager	= engine::ScriptManager::Instance();
			auto& script_system		= script_manager.m_script_system;
			auto& style				= ImGui::GetStyle();

			auto cheld_size_x = 220.f;

			auto button_size_x = ( cheld_size_x - style.ItemSpacing.x ) / 2.f;
			auto button_size_y = 25.f;

			ImGui::SetNextWindowSize( ImVec2( m_start_width , m_start_height ) );
			//ImGui::SetNextWindowPos( ImVec2( 0.f , (float)iScreenHeight - m_start_height ) , ImGuiSetCond_Once );
			ImGui::SetNextWindowPos( ImVec2( 0.f , (float)iScreenHeight - m_start_height ) , ImGuiSetCond_FirstUseEver );

			if ( ImGui::Begin( XorStr( "Andromeda DLC" ) , 0 , ImGuiWindowFlags_NoResize ) )
			{
				ImGui::BeginGroup();
				ImGui::BeginChild( XorStr( "AndromedaChildLeft" ) , ImVec2( cheld_size_x , ImGui::GetContentRegionAvail().y - button_size_y - style.ItemSpacing.y ) , true );

				if ( m_select_script_index > script_system->m_module_list.size() )
					m_select_script_index = script_system->m_module_list.size() - 1;
				
				for ( size_t Index = 0; Index < script_system->m_module_list.size(); Index++ )
				{
					auto module = script_system->m_module_list.at( Index );
					auto script_module = module.m_script_module;
					
					if ( !script_module )
						continue;

					string module_name = script_module->GetName();

					ImColor script_status_color( 255 , 255 , 255 );

					if ( module.m_enable )
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

					/*if ( ImGui::MenuItem( XorStr( "Edit Script##ModuleList" ) ) )
					{

					}*/

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
				ImGui::BeginChild( XorStr( "AndromedaChildRightTop" ) , ImVec2( cheld_size_x , ImGui::GetContentRegionAvail().y ) , true );
				
				if ( ButtonIcon( ICON_FA_CROSSHAIRS , XorStr( "Aimbot (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Aimbot Menu Clicked !" );
				}

				if ( ButtonIcon( ICON_FA_PLUS_CIRCLE, XorStr( "TriggerBot (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
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

				if ( ButtonIcon( ICON_FA_COG , XorStr( "Settings (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "Settings Menu Clicked !" );
				}

				if ( ButtonIcon( ICON_FA_INFO_CIRCLE , XorStr( "About (null)##AndromedaChildRightBottom" ) , ImVec2( -1.f , button_size_y ) ) )
				{
					notify.AddNotification( 5 , nt_info , "%s" , "About Menu Clicked !" );
				}

				ImGui::EndChild();
				ImGui::EndGroup();
				
				ImGui::End();
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
	}
}