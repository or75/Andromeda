#include "Gui.hpp"

LRESULT ImGui_ImplWin32_WndProcHandler( HWND window , UINT message , WPARAM wparam , LPARAM lparam );

namespace source
{
	namespace feature
	{
		auto Gui::Create( IDirect3DDevice9* direct_device , Andromeda::Input& input ) -> bool
		{
			m_gui_file = Andromeda::ImageLoader::Instance().GetDllDir() + XorStr( "gui.ini" );
			m_gui_open = false;
			m_render_menu_fn = nullptr;
			m_gui_create = false;

			input.AddEvent( &Gui::Event );

			ImGui::CreateContext();

			ImGui::GetIO().IniFilename = m_gui_file.c_str();
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

			ImGui_ImplWin32_Init( input.GetWindow() );
			ImGui_ImplDX9_Init( direct_device );

			InitStyle();

			m_gui_create = InitFont();
			return m_gui_create;
		}

		auto Gui::Destroy() -> void
		{
			m_gui_create = false;
			m_gui_open = false;
			m_render_menu_fn = nullptr;

			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext();
		}

		auto Gui::AddMenuRenderFn( GuiRenderMenuFn Fn ) -> void
		{
			m_render_menu_fn = Fn;
		}

		auto Gui::InitFont() -> bool
		{
			ImGuiIO& io = ImGui::GetIO();

			io.Fonts->ClearFonts();

			ImFontConfig FontUnicodeConfig;
			ImFontConfig FontAwesomeIconConfig;

			FontUnicodeConfig.OversampleH = 1;
			FontUnicodeConfig.OversampleV = 1;
			FontUnicodeConfig.PixelSnapH = true;

			FontAwesomeIconConfig.OversampleH = 1;
			FontAwesomeIconConfig.OversampleV = 1;
			FontAwesomeIconConfig.PixelSnapH = true;

			static const ImWchar UnicodeRanges[] =
			{
				0x0021, 0xFFFD,
				0
			};

			static const ImWchar AwesomeIconRanges[] =
			{
				0x002D, 0x007A,
				ICON_MIN_FA, ICON_MAX_FA,
				0
			};

			string font_unicode_ms_file = "";

			/*char szWindowFolder[MAX_PATH] = { 0 };

			if ( GetWindowsDirectoryA( szWindowFolder , MAX_PATH ) )
			{
				font_unicode_ms_file = szWindowFolder;
				font_unicode_ms_file += XorStr( "\\Fonts\\ARIALUNI.ttf" );

				if ( GetFileAttributesA( font_unicode_ms_file.c_str() ) == INVALID_FILE_ATTRIBUTES )
				{
					font_unicode_ms_file = Andromeda::ImageLoader::Instance().GetDllDir() + XorStr( "ARIALUNI.ttf" );
				}
			}*/

			font_unicode_ms_file = Andromeda::ImageLoader::Instance().GetDllDir() + XorStr( "ARIALUNI.ttf" );

			if ( GetFileAttributesA( font_unicode_ms_file.c_str() ) != INVALID_FILE_ATTRIBUTES )
			{
				m_font_unicode_ms = io.Fonts->AddFontFromFileTTF( font_unicode_ms_file.c_str() , 17.f , &FontUnicodeConfig , UnicodeRanges );
				m_font_unicode_ms->DisplayOffset.y -= 1.f;

				m_font_awesome_icon = io.Fonts->AddFontFromMemoryCompressedTTF( FontAwesomeIcon_compressed_data , FontAwesomeIcon_compressed_size , 25.f , &FontAwesomeIconConfig , AwesomeIconRanges );
				//m_font_awesome_icon->DisplayOffset.y -= 1.f;

				font_unicode_ms_file = "";

				return true;
			}
			else
			{
				Andromeda::WriteDebugLog( XorStr( "[error] #FontUnicodeMS\n" ) );
			}

			return false;
		}

		auto Gui::InitStyle() -> void
		{
			auto& style = ImGui::GetStyle();
			auto& colors = style.Colors;

			float roundness = 2.0f;

			style.WindowBorderSize = 1.0f;
			style.FrameBorderSize = 1.0f;
			style.WindowMinSize = ImVec2( 75.f , 50.f );
			style.FramePadding = ImVec2( 5 , 5 );
			style.ItemSpacing = ImVec2( 6 , 5 );
			style.ItemInnerSpacing = ImVec2( 2 , 4 );
			style.Alpha = 1.0f;
			style.WindowRounding = 0.f;
			style.FrameRounding = roundness;
			style.PopupRounding = 0;
			style.PopupBorderSize = 2;
			style.IndentSpacing = 6.0f;
			style.ColumnsMinSpacing = 50.0f;
			style.GrabMinSize = 14.0f;
			style.GrabRounding = roundness;
			style.ScrollbarSize = 12.0f;
			style.ScrollbarRounding = roundness;

			style.AntiAliasedFill = true;
			style.AntiAliasedLines = true;

			colors[ImGuiCol_Text] = ImVec4( 1.00f , 1.00f , 1.00f , 1.00f );
			colors[ImGuiCol_TextDisabled] = ImVec4( 0.50f , 0.50f , 0.50f , 1.00f );
			
			colors[ImGuiCol_WindowBg] = ImVec4( 0.20f , 0.23f , 0.31f , 1.00f );
			colors[ImGuiCol_ChildBg] = ImVec4( 0.20f , 0.23f , 0.31f , 1.00f );
			colors[ImGuiCol_PopupBg] = ImVec4( 0.20f , 0.23f , 0.31f , 1.00f );

			colors[ImGuiCol_Border] = ImVec4( 0.25f , 0.28f , 0.38f , 1.00f );
			colors[ImGuiCol_BorderShadow] = ImVec4( 0.00f , 0.00f , 0.00f , 0.00f );
			
			colors[ImGuiCol_FrameBg] = ImVec4( 0.25f , 0.28f , 0.38f , 1.00f );
			colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.25f , 0.28f , 0.38f , 1.00f );
			colors[ImGuiCol_FrameBgActive] = ImVec4( 0.25f , 0.28f , 0.38f , 1.00f );
			
			colors[ImGuiCol_TitleBg] = ImVec4( 0.00f , 0.43f , 1.00f , 1.00f );
			colors[ImGuiCol_TitleBgActive] = ImVec4( 0.00f , 0.55f , 1.00f , 1.00f );
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.10f , 0.69f , 1.00f , 1.00f );
			
			colors[ImGuiCol_MenuBarBg] = ImVec4( 0.25f , 0.28f , 0.38f , 1.00f );
			
			colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.00f , 0.00f , 0.00f , 0.00f );
			colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.39f , 0.44f , 0.56f , 1.00f );
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.39f , 0.44f , 0.56f , 1.00f );
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.39f , 0.44f , 0.56f , 1.00f );
			
			colors[ImGuiCol_CheckMark] = ImVec4( 0.00f , 0.55f , 1.00f , 1.00f );
			
			colors[ImGuiCol_SliderGrab] = ImVec4( 0.00f , 0.55f , 1.00f , 1.00f );
			colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.10f , 0.69f , 1.00f , 1.00f );
			
			colors[ImGuiCol_Button] = ImVec4( 0.25f , 0.28f , 0.38f , 1.00f );
			colors[ImGuiCol_ButtonHovered] = ImVec4( 0.12f , 0.43f , 1.00f , 1.00f );
			colors[ImGuiCol_ButtonActive] = ImVec4( 0.00f , 0.55f , 1.00f , 1.00f );
			
			colors[ImGuiCol_Header] = ImVec4( 0.00f , 0.43f , 1.00f , 1.00f );
			colors[ImGuiCol_HeaderHovered] = ImVec4( 0.00f , 0.55f , 1.00f , 1.00f );
			colors[ImGuiCol_HeaderActive] = ImVec4( 0.00f , 0.43f , 1.00f , 1.00f );
			
			colors[ImGuiCol_Separator] = ImVec4( 0.43f , 0.43f , 0.50f , 0.50f );
			colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.10f , 0.40f , 0.75f , 0.78f );
			colors[ImGuiCol_SeparatorActive] = ImVec4( 0.10f , 0.40f , 0.75f , 1.00f );
			
			colors[ImGuiCol_ResizeGrip] = ImVec4( 0.26f , 0.59f , 0.98f , 0.25f );
			colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.26f , 0.59f , 0.98f , 0.67f );
			colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.26f , 0.59f , 0.98f , 0.95f );
			
			colors[ImGuiCol_Tab] = ImVec4( 0.00f , 0.50f , 1.00f , 1.00f );
			colors[ImGuiCol_TabHovered] = ImVec4( 0.12f , 0.69f , 1.00f , 1.00f );
			
			colors[ImGuiCol_TabActive] = ImVec4( 0.12f , 0.69f , 1.00f , 1.00f );
			colors[ImGuiCol_TabUnfocused] = ImVec4( 0.07f , 0.10f , 0.15f , 0.97f );
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4( 0.14f , 0.26f , 0.42f , 1.00f );
			
			colors[ImGuiCol_PlotLines] = ImVec4( 0.61f , 0.61f , 0.61f , 1.00f );
			colors[ImGuiCol_PlotLinesHovered] = ImVec4( 1.00f , 0.43f , 0.35f , 1.00f );
			colors[ImGuiCol_PlotHistogram] = ImVec4( 0.10f , 0.69f , 1.00f , 1.00f );
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 1.00f , 0.60f , 0.00f , 1.00f );
			
			colors[ImGuiCol_TextSelectedBg] = ImVec4( 0.26f , 0.59f , 0.98f , 0.35f );
			colors[ImGuiCol_DragDropTarget] = ImVec4( 1.00f , 1.00f , 0.00f , 0.90f );
			
			colors[ImGuiCol_NavHighlight] = ImVec4( 0.26f , 0.59f , 0.98f , 1.00f );
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 1.00f , 1.00f , 1.00f , 0.70f );
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4( 0.80f , 0.80f , 0.80f , 0.20f );
			
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4( 0.80f , 0.80f , 0.80f , 0.35f );
		}

		auto Gui::Begin() -> void
		{
			if ( m_gui_create )
			{
				ImGui_ImplDX9_NewFrame();
				ImGui_ImplWin32_NewFrame();

				ImGui::NewFrame();

				if ( m_gui_open )
				{
					if ( m_render_menu_fn )
					{
						m_render_menu_fn();
					}
				}
			}
		}

		auto Gui::End() -> void
		{
			if ( m_gui_create )
			{
				ImGui::EndFrame();
				ImGui::Render();

				ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
			}
		}

		auto Gui::DeviceLost() -> void
		{
			if ( m_gui_create )
				ImGui_ImplDX9_InvalidateDeviceObjects();
		}

		auto Gui::DeviceReset() -> void
		{
			if ( m_gui_create )
				ImGui_ImplDX9_CreateDeviceObjects();
		}

		auto Gui::Event( UINT message , WPARAM wparam , LPARAM lparam ) -> bool
		{
			auto& input = Andromeda::Input::Instance();
			auto& gui = Gui::Instance();

			if ( message == WM_KEYUP && wparam == VK_INSERT )
			{
				gui.m_gui_open = !gui.m_gui_open;

				ShowCursor( !gui.m_gui_open );
				source::m_input_system->EnableInput( !gui.m_gui_open );
				ImGui::GetIO().MouseDrawCursor = gui.m_gui_open;
			}

			if ( gui.m_gui_open && ImGui_ImplWin32_WndProcHandler( input.GetWindow() , message , wparam , lparam ) == 0 )
				return true;

			return false;
		}
	}
}