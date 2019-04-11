#include "Config.hpp"

#include "../Include/Rapidjson/rapidjson.h"
#include "../Include/Rapidjson/prettywriter.h"
#include "../Include/Rapidjson/document.h"
#include "../Include/Rapidjson/istreamwrapper.h"
#include "../Include/Rapidjson/ostreamwrapper.h"
#include "../Include/Rapidjson/error/en.h"

namespace source
{
	using namespace rapidjson;

	void GetFloatJson( Value& style, const char* name , float& output )
	{
		if ( !style.IsNull() && style.HasMember( name ) )
		{		
			auto& value = style[name];

			if ( !value.IsNull() && value.IsFloat() )
			{
				output = value.GetFloat();

				Andromeda::WriteDebugLog( "[%s] , %0.2f\n" , name , output );
			}
		}
	}

	void GetImVec2Json( Value& style , const char* name , ImVec2& output )
	{
		if ( !style.IsNull() && style.HasMember( name ) )
		{
			auto& value = style[name];

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

	void GetImVec4Json( Value& style , const char* name , ImVec4& output )
	{
		if ( !style.IsNull() && style.HasMember( name ) )
		{
			auto& value = style[name];

			if ( !value.IsNull() && value.IsArray() )
			{
				auto Array_ImVec4 = value.GetArray();

				if ( Array_ImVec4.Size() == 4 && Array_ImVec4[0].IsFloat() && Array_ImVec4[1].IsFloat() && Array_ImVec4[2].IsFloat() && Array_ImVec4[3].IsFloat() )
				{
					output.x = Array_ImVec4[0].GetFloat();
					output.y = Array_ImVec4[1].GetFloat();
					output.z = Array_ImVec4[2].GetFloat();
					output.w = Array_ImVec4[3].GetFloat();

					Andromeda::WriteDebugLog( "[%s] , %0.2f , %0.2f , %0.2f , %0.2f\n" , name , output.x , output.y , output.z , output.w );
				}
			}
		}
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

	void LoadConfig( const char* file_name )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		ifstream cfg_file( dll_dir + file_name );
		Document doc_config;
		IStreamWrapper isw( cfg_file );

		doc_config.ParseStream( isw );

		if ( doc_config.HasParseError() )
		{
			Andromeda::WriteDebugLog( "[error] load json: %s (%i)\n" , GetParseError_En( doc_config.GetParseError() ) , doc_config.GetErrorOffset() );
		}
		else
		{
			// ImGui
			{
				auto& ImGui_Style = ImGui::GetStyle();
				auto& ImGui_Colors = ImGui_Style.Colors;

				auto& imgui_style_json = doc_config[XorStr( "ImGui" )][XorStr( "style" )];
				auto& imgui_color_json = doc_config[XorStr( "ImGui" )][XorStr( "color" )];

				if ( !imgui_style_json.IsNull() )
				{
					GetFloatJson( imgui_style_json , XorStr( "WindowBorderSize" ) , ImGui_Style.WindowBorderSize );
					GetFloatJson( imgui_style_json , XorStr( "FrameBorderSize" ) , ImGui_Style.FrameBorderSize );

					GetImVec2Json( imgui_style_json , XorStr( "FramePadding" ) , ImGui_Style.FramePadding );
					GetImVec2Json( imgui_style_json , XorStr( "ItemSpacing" ) , ImGui_Style.ItemSpacing );
					GetImVec2Json( imgui_style_json , XorStr( "ItemInnerSpacing" ) , ImGui_Style.ItemInnerSpacing );

					GetFloatJson( imgui_style_json , XorStr( "Alpha" ) , ImGui_Style.Alpha );
					GetFloatJson( imgui_style_json , XorStr( "WindowRounding" ) , ImGui_Style.WindowRounding );
					GetFloatJson( imgui_style_json , XorStr( "FrameRounding" ) , ImGui_Style.FrameRounding );

					GetFloatJson( imgui_style_json , XorStr( "PopupRounding" ) , ImGui_Style.PopupRounding );
					GetFloatJson( imgui_style_json , XorStr( "PopupBorderSize" ) , ImGui_Style.PopupBorderSize );

					GetFloatJson( imgui_style_json , XorStr( "IndentSpacing" ) , ImGui_Style.IndentSpacing );

					GetFloatJson( imgui_style_json , XorStr( "GrabMinSize" ) , ImGui_Style.GrabMinSize );
					GetFloatJson( imgui_style_json , XorStr( "GrabRounding" ) , ImGui_Style.GrabRounding );

					GetFloatJson( imgui_style_json , XorStr( "ScrollbarSize" ) , ImGui_Style.ScrollbarSize );
					GetFloatJson( imgui_style_json , XorStr( "ScrollbarRounding" ) , ImGui_Style.ScrollbarRounding );
				}

				if ( !imgui_color_json.IsNull() )
				{
					for ( size_t idx = ImGuiCol_Text; idx < ImGuiCol_COUNT; idx++ )
						GetImVec4Json( imgui_style_json , ImGui::GetStyleColorName( idx ) , ImGui_Colors[ImGuiCol_Text] );
				}
			}
		}

		doc_config.Clear();

		cfg_file.clear();
		cfg_file.close();
	}

	void SaveConfig( const char* file_name )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		ofstream cfg_file( dll_dir + file_name );

		OStreamWrapper osw( cfg_file );
		PrettyWriter<OStreamWrapper> cfg_writer( osw );

		cfg_writer.SetIndent( '\t' , 1 );
		cfg_writer.SetFormatOptions( PrettyFormatOptions::kFormatSingleLineArray );
		cfg_writer.SetMaxDecimalPlaces( 2 );

		{
			cfg_writer.StartObject();

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
	}
}