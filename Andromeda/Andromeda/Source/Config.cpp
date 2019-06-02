#include "Config.hpp"

#include "../Include/Rapidjson/rapidjson.h"
#include "../Include/Rapidjson/prettywriter.h"
#include "../Include/Rapidjson/document.h"
#include "../Include/Rapidjson/istreamwrapper.h"
#include "../Include/Rapidjson/ostreamwrapper.h"
#include "../Include/Rapidjson/error/en.h"

#include "Feature/Notification.hpp"

namespace config
{
	namespace settings
	{
		namespace main
		{
			int WelcomeMsg = 1;
			int Timeout = 300;
			int MenuKey = VK_INSERT;
		}
	}
}

namespace source
{
	using namespace rapidjson;

	void GetIntJson( Value& json , const char* name , int& output , int min , int max )
	{
		if ( !json.IsNull() && json.HasMember( name ) )
		{
			auto& value = json[name];

			if ( !value.IsNull() && value.IsInt() )
			{
				auto f = value.GetInt();

				if ( f >= min && f <= max )
					output = f;
			}
		}
	}

	void GetBoolJson( Value& json , const char* name , int& output )
	{
		if ( !json.IsNull() && json.HasMember( name ) )
		{
			auto& value = json[name];

			if ( !value.IsNull() && value.IsBool() )
			{
				output = value.GetBool() ? 1 : 0;
			}
		}
	}

	void GetFloatJson( Value& json , const char* name , float& output , float min , float max )
	{
		if ( !json.IsNull() && json.HasMember( name ) )
		{
			auto& value = json[name];

			if ( !value.IsNull() && value.IsFloat() )
			{
				auto f = value.GetFloat();

				if ( f >= min && f <= max )
					output = f;
			}
		}
	}

	void GetImVec2Json( Value& json , const char* name , ImVec2& output )
	{
		if ( !json.IsNull() && json.HasMember( name ) )
		{
			auto& value = json[name];

			if ( !value.IsNull() && value.IsArray() )
			{
				auto Array_ImVec2 = value.GetArray();

				if ( Array_ImVec2.Size() == 2 && Array_ImVec2[0].IsFloat() && Array_ImVec2[1].IsFloat() )
				{
					output.x = Array_ImVec2[0].GetFloat();
					output.y = Array_ImVec2[1].GetFloat();
				}
			}
		}
	}

	void GetImVec4Json( Value& json , const char* name , ImVec4& output )
	{
		if ( !json.IsNull() && json.HasMember( name ) )
		{
			auto& value = json[name];

			if ( !value.IsNull() && value.IsArray() )
			{
				auto Array_ImVec4 = value.GetArray();

				if ( Array_ImVec4.Size() == 4 && Array_ImVec4[0].IsFloat() && Array_ImVec4[1].IsFloat() && Array_ImVec4[2].IsFloat() && Array_ImVec4[3].IsFloat() )
				{
					output.x = Array_ImVec4[0].GetFloat();
					output.y = Array_ImVec4[1].GetFloat();
					output.z = Array_ImVec4[2].GetFloat();
					output.w = Array_ImVec4[3].GetFloat();
				}
			}
		}
	}

	void AddIntJson( PrettyWriter<OStreamWrapper>& writer , const char* name , int& output )
	{
		writer.String( name );
		writer.Int( output );
	}

	void AddBoolJson( PrettyWriter<OStreamWrapper>& writer , const char* name , int& output )
	{
		writer.String( name );
		writer.Bool( ( output ? true : false ) );
	}

	void AddImGuiStyleJson( PrettyWriter<OStreamWrapper>& writer , const char* name , float& value )
	{
		writer.String( name );
		writer.Double( value );
	}

	void AddImGuiImVec2Json( PrettyWriter<OStreamWrapper>& writer , const char* name , ImVec2& color )
	{
		writer.String( name );

		writer.StartArray();

		writer.Double( color.x );
		writer.Double( color.y );

		writer.EndArray();
	}

	void AddImGuiImVec4Json( PrettyWriter<OStreamWrapper>& writer , const char* name , ImVec4& color )
	{
		writer.String( name );

		writer.StartArray();

		writer.Double( color.x );
		writer.Double( color.y );
		writer.Double( color.z );
		writer.Double( color.w );

		writer.EndArray();
	}

	void LoadConfig( string file_name , bool notifi )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();
		auto& notify = feature::Notification::Instance();

		wstring cfg_full_file_name = Andromeda::ansi_to_unicode( dll_dir + file_name );

		ifstream cfg_file( cfg_full_file_name );
		Document doc_config;
		IStreamWrapper isw( cfg_file );

		doc_config.ParseStream( isw );

		if ( doc_config.HasParseError() )
		{
			Andromeda::WriteDebugLog( "[error] load (%ws) json: %s (%i)\n" , cfg_full_file_name.c_str() , GetParseError_En( doc_config.GetParseError() ) , doc_config.GetErrorOffset() );
		}
		else
		{
			// Script
			{
				auto& script_json = doc_config[XorStr( "Settings" )][XorStr( "Main" )];

				if ( !script_json.IsNull() )
				{
					GetBoolJson( script_json , XorStr( "WelcomeMsg" ) , config::settings::main::WelcomeMsg );
					GetIntJson( script_json , XorStr( "Timeout" ) , config::settings::main::Timeout , 150 , 5000 );	
				}
			}

			// ImGui
			{
				auto& ImGui_Style = ImGui::GetStyle();
				auto& ImGui_Colors = ImGui_Style.Colors;

				auto& imgui_style_json = doc_config[XorStr( "ImGui" )][XorStr( "style" )];
				auto& imgui_color_json = doc_config[XorStr( "ImGui" )][XorStr( "color" )];

				if ( !imgui_style_json.IsNull() )
				{
					GetFloatJson( imgui_style_json , XorStr( "WindowBorderSize" ) , ImGui_Style.WindowBorderSize , 0.f , 10.f );
					GetFloatJson( imgui_style_json , XorStr( "FrameBorderSize" ) , ImGui_Style.FrameBorderSize , 0.f , 10.f );

					GetImVec2Json( imgui_style_json , XorStr( "FramePadding" ) , ImGui_Style.FramePadding );
					GetImVec2Json( imgui_style_json , XorStr( "ItemSpacing" ) , ImGui_Style.ItemSpacing );
					GetImVec2Json( imgui_style_json , XorStr( "ItemInnerSpacing" ) , ImGui_Style.ItemInnerSpacing );

					GetFloatJson( imgui_style_json , XorStr( "Alpha" ) , ImGui_Style.Alpha , 0.f , 1.f );
					GetFloatJson( imgui_style_json , XorStr( "WindowRounding" ) , ImGui_Style.WindowRounding , 0.f , 10.f );
					GetFloatJson( imgui_style_json , XorStr( "FrameRounding" ) , ImGui_Style.FrameRounding , 0.f , 10.f );

					GetFloatJson( imgui_style_json , XorStr( "PopupRounding" ) , ImGui_Style.PopupRounding , 0.f , 10.f );
					GetFloatJson( imgui_style_json , XorStr( "PopupBorderSize" ) , ImGui_Style.PopupBorderSize , 0.f , 10.f );

					GetFloatJson( imgui_style_json , XorStr( "IndentSpacing" ) , ImGui_Style.IndentSpacing , 0.f , 10.f );

					GetFloatJson( imgui_style_json , XorStr( "GrabMinSize" ) , ImGui_Style.GrabMinSize , 0.f , 10.f );
					GetFloatJson( imgui_style_json , XorStr( "GrabRounding" ) , ImGui_Style.GrabRounding , 0.f , 10.f );

					GetFloatJson( imgui_style_json , XorStr( "ScrollbarSize" ) , ImGui_Style.ScrollbarSize , 0.f , 10.f );
					GetFloatJson( imgui_style_json , XorStr( "ScrollbarRounding" ) , ImGui_Style.ScrollbarRounding , 0.f , 10.f );
				}

				if ( !imgui_color_json.IsNull() )
				{
					for ( size_t idx = ImGuiCol_Text; idx < ImGuiCol_COUNT; idx++ )
						GetImVec4Json( imgui_style_json , ImGui::GetStyleColorName( idx ) , ImGui_Colors[ImGuiCol_Text] );
				}
			}

			if ( notifi )
			{
				string message = "'" + file_name + XorStr( "' loaded !" );
				notify.AddNotification( 5 , feature::nt_success , message.c_str() );
			}
		}

		doc_config.Clear();

		cfg_file.clear();
		cfg_file.close();
	}

	void SaveConfig( string file_name , bool notifi )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();
		auto& notify = feature::Notification::Instance();

		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		wstring cfg_full_file_name = Andromeda::ansi_to_unicode( dll_dir + file_name );

		ofstream cfg_file( cfg_full_file_name );

		OStreamWrapper osw( cfg_file );
		PrettyWriter<OStreamWrapper> cfg_writer( osw );

		cfg_writer.SetIndent( '\t' , 1 );
		cfg_writer.SetFormatOptions( PrettyFormatOptions::kFormatSingleLineArray );
		cfg_writer.SetMaxDecimalPlaces( 2 );

		{
			cfg_writer.StartObject();

			cfg_writer.String( XorStr( "Settings" ) );
			{
				cfg_writer.StartObject();

				cfg_writer.String( XorStr( "Main" ) );
				{
					cfg_writer.StartObject();

					AddBoolJson( cfg_writer , XorStr( "WelcomeMsg" ) , config::settings::main::WelcomeMsg );
					AddIntJson( cfg_writer , XorStr( "Timeout" ) , config::settings::main::Timeout );

					cfg_writer.EndObject();
				}

				cfg_writer.EndObject();
			}

			cfg_writer.String( XorStr( "ImGui" ) );
			{
				cfg_writer.StartObject();

				cfg_writer.String( XorStr( "style" ) );
				{
					cfg_writer.StartObject();

					AddImGuiStyleJson( cfg_writer , XorStr( "WindowBorderSize" ) , style.WindowBorderSize );
					AddImGuiStyleJson( cfg_writer , XorStr( "FrameBorderSize" ) , style.FrameBorderSize );

					AddImGuiImVec2Json( cfg_writer , XorStr( "FramePadding" ) , style.FramePadding );
					AddImGuiImVec2Json( cfg_writer , XorStr( "ItemSpacing" ) , style.ItemSpacing );
					AddImGuiImVec2Json( cfg_writer , XorStr( "ItemInnerSpacing" ) , style.ItemInnerSpacing );

					AddImGuiStyleJson( cfg_writer , XorStr( "Alpha" ) , style.Alpha );
					AddImGuiStyleJson( cfg_writer , XorStr( "WindowRounding" ) , style.WindowRounding );
					AddImGuiStyleJson( cfg_writer , XorStr( "FrameRounding" ) , style.FrameRounding );

					AddImGuiStyleJson( cfg_writer , XorStr( "PopupRounding" ) , style.PopupRounding );
					AddImGuiStyleJson( cfg_writer , XorStr( "PopupBorderSize" ) , style.PopupBorderSize );

					AddImGuiStyleJson( cfg_writer , XorStr( "IndentSpacing" ) , style.IndentSpacing );

					AddImGuiStyleJson( cfg_writer , XorStr( "GrabMinSize" ) , style.GrabMinSize );
					AddImGuiStyleJson( cfg_writer , XorStr( "GrabRounding" ) , style.GrabRounding );

					AddImGuiStyleJson( cfg_writer , XorStr( "ScrollbarSize" ) , style.ScrollbarSize );
					AddImGuiStyleJson( cfg_writer , XorStr( "ScrollbarRounding" ) , style.ScrollbarRounding );

					cfg_writer.EndObject();
				}

				cfg_writer.String( XorStr( "color" ) );
				{
					cfg_writer.StartObject();

					for ( size_t idx = ImGuiCol_Text; idx < ImGuiCol_COUNT; idx++ )
						AddImGuiImVec4Json( cfg_writer , ImGui::GetStyleColorName( idx ) , colors[idx] );
					
					cfg_writer.EndObject();
				}

				cfg_writer.EndObject();
			}

			cfg_writer.EndObject();
		}

		cfg_file.clear();
		cfg_file.close();

		if ( notifi )
		{
			string message = "'" + file_name + XorStr( "' saved !" );
			notify.AddNotification( 5 , feature::nt_success , message.c_str() );
		}
	}
}