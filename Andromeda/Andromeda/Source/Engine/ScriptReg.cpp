#include "ScriptReg.hpp"

#include "../../Valve/Source/Entity.hpp"
#include "../../Valve/Source/Player.hpp"
#include "../../Valve/Source/Weapon.hpp"

#include "../Feature/Notification.hpp"

#include "../Config.hpp"

namespace source
{
	namespace engine
	{

#pragma region Parse

		void asParseVarArgs( asIScriptGeneric* Gen , int ArgIndex , char* Buffer , size_t Size )
		{
			auto engine = Gen->GetEngine();

			memset( Buffer , 0 , Size );

			for ( int i = ArgIndex; i < Gen->GetArgCount(); i++ )
			{
				PVOID addr = *(PVOID *)Gen->GetAddressOfArg( i );

				int type = Gen->GetArgTypeId( i );
				int size = engine->GetSizeOfPrimitiveType( type );

#define MAKE_TYPE(id, typee) \
			case id: \
			va_arg(Buffer, typee) = *(typee*)addr; \
			break;

				switch ( type )
				{
					case asTYPEID_VOID:
						continue;
					case asTYPEID_FLOAT:
						va_arg( Buffer , double ) = static_cast<double>( *(float*)addr );
						break;
					case asTYPEID_DOUBLE:
						va_arg( Buffer , double ) = *(double*)addr;
						break;

					MAKE_TYPE( asTYPEID_BOOL , bool );
					MAKE_TYPE( asTYPEID_UINT8 , unsigned __int8 );
					MAKE_TYPE( asTYPEID_INT8 , __int8 );
					MAKE_TYPE( asTYPEID_UINT16 , unsigned __int16 );
					MAKE_TYPE( asTYPEID_INT16 , __int16 );
					MAKE_TYPE( asTYPEID_UINT32 , unsigned __int32 );
					MAKE_TYPE( asTYPEID_INT32 , __int32 );
					MAKE_TYPE( asTYPEID_UINT64 , unsigned __int64 );
					MAKE_TYPE( asTYPEID_INT64 , __int64 );

					default:
					{
						// if (object is a string)
						if ( type == engine->GetTypeIdByDecl( XorStr( "string" ) ) )
						{
							// std::string
							va_arg( Buffer , const char * ) = ( (string *)addr )->c_str();
						}
						else
						{
							// Handle custom enumeration values
							if ( size == sizeof( int ) )
							{
								va_arg( Buffer , int ) = *(int *)addr;
							}
							else
							{
								auto TypeDecl = engine->GetTypeDeclaration( type );

								if ( TypeDecl )
									Andromeda::WriteDebugLog( XorStr( "[error] ParseVarArgs unknown type arg: %s\n" ) , TypeDecl );
							}
						}
					}

					break;
				}
#undef MAKE_TYPE
			}
		}

		void asParseFormat( asIScriptGeneric* Gen , int FormatArgIndex , char* Buffer , size_t Size )
		{
			// Format is the first parameter
			string *format = static_cast<string *>( Gen->GetArgObject( FormatArgIndex ) );

			// Buffer to store arguments for vsnprintf_s
			char data[1024] = { 0 };

			asParseVarArgs( Gen , FormatArgIndex + 1 , data , sizeof( data ) );

			// Send back to caller
			vsnprintf_s( Buffer , Size , _TRUNCATE , format->c_str() , (va_list)&data );
		}


#pragma endregion

#pragma region Global

		void asSprintf( asIScriptGeneric* Gen )
		{
			char buf[2048] = { 0 };
			asParseFormat( Gen , 0 , buf , ARRAYSIZE( buf ) );

			// Set the return data
			string ret( buf );

			Gen->SetReturnObject( &ret );
		}

		void asLog( asIScriptGeneric* Gen )
		{
			char buf[2048] = { 0 };
			asParseFormat( Gen , 0 , buf , ARRAYSIZE( buf ) );

			Andromeda::WriteDebugLog( buf );
		}

		auto asGetModuleName() -> string
		{
			auto context = asGetActiveContext();

			if ( context )
			{
				string module_name = Andromeda::str_wide_to_str_unicode( context->GetFunction()->GetModuleName() );
				return module_name;
			}

			return string( "" );
		}

		auto asGetScriptDir() -> string
		{
			auto& script_manager = ScriptManager::Instance();

			return Andromeda::str_wide_to_str_unicode( script_manager.m_script_dir );
		}

		auto asPlaySound( string sound_file )
		{
			string sound_file_name = XorStr( "\\andromeda\\" ) + sound_file;
			
			m_surface->PlaySoundFile( sound_file_name.c_str() );
		}

		auto asAddNotification( asIScriptGeneric* Gen )
		{
			auto& notify = feature::Notification::Instance();

			auto show_sec = *(asDWORD*)Gen->GetAddressOfArg( 0 );
			auto type = *(asDWORD*)Gen->GetAddressOfArg( 1 );

			char buf[2048] = { 0 };

			asParseFormat( Gen , 2 , buf , ARRAYSIZE( buf ) );

			string message = buf;

			notify.AddNotification( show_sec , ( feature::notify_type )type , message.c_str() );
		}

#pragma endregion

#pragma region Vector2

		void Vector2_Constructor( void* memory )
		{
			new( memory ) Vector2();
		}

		void Vector2_InitConstructor( float x , float y , Vector2* self )
		{
			new( self ) Vector2( x , y );
		}

		void Vector2_CopyConstructor( const Vector2& other , Vector2* self )
		{
			new( self ) Vector2( other );
		}

		void Vector2_ListConstructor( float* list , Vector2* self )
		{
			new( self ) Vector2( list[0] , list[1] );
		}

#pragma endregion

#pragma region Vector3

		void Vector3_Constructor( void* memory )
		{
			new( memory ) Vector3();
		}

		void Vector3_InitConstructor( float x , float y , float z , Vector3* self )
		{
			new( self ) Vector3( x , y , z );
		}

		void Vector3_CopyConstructor( const Vector3& other , Vector3* self )
		{
			new( self ) Vector3( other );
		}

		void Vector3_ListConstructor( float* list , Vector3* self )
		{
			new( self ) Vector3( list[0] , list[1] , list[2] );
		}

#pragma endregion

#pragma region QAngle

		void QAngle_Constructor( void* memory )
		{
			new( memory ) QAngle();
		}

		void QAngle_InitConstructor( float x , float y , float z , QAngle* self )
		{
			new( self ) QAngle( x , y , z );
		}

		void QAngle_CopyConstructor( const QAngle& other , QAngle* self )
		{
			new( self ) QAngle( other );
		}

		void QAngle_ListConstructor( float* list , QAngle* self )
		{
			new( self ) QAngle( list[0] , list[1] , list[2] );
		}

#pragma endregion

#pragma region Color

		void Color_Constructor( void* memory )
		{
			new( memory ) Color();
		}

		void Color_InitConstructor( BYTE r , BYTE g , BYTE b , BYTE a , Color* self )
		{
			new( self ) Color( r , g , b , a );
		}

		void Color_InitConstructor( D3DCOLOR c , Color* self )
		{
			new( self ) Color( c );
		}

		void Color_CopyConstructor( const Color& other , Color* self )
		{
			new( self ) Color( other );
		}

#pragma endregion

#pragma region Render

		void asDrawText( asIScriptGeneric* Gen )
		{
			auto& render = engine::Render::Instance();

			auto font_index = *(asDWORD*)Gen->GetAddressOfArg( 0 );

			auto x = *(PINT)Gen->GetAddressOfArg( 1 );
			auto y = *(PINT)Gen->GetAddressOfArg( 2 );

			auto font_align = *(asDWORD*)Gen->GetAddressOfArg( 3 );

			auto color = *(Color*)Gen->GetArgObject( 4 );

			char buf[2048] = { 0 };
			asParseFormat( Gen , 5 , buf , ARRAYSIZE( buf ) );

			render.Text( font_index , x , y , font_align , color , buf );
		}

		void asGetTextSize( asIScriptGeneric* Gen )
		{
			auto& render = engine::Render::Instance();

			asDWORD font_index = *(asDWORD*)Gen->GetAddressOfArg( 0 );

			auto& w = *(PINT)Gen->GetArgAddress( 1 );
			auto& h = *(PINT)Gen->GetArgAddress( 2 );

			char buf[2048] = { 0 };
			asParseFormat( Gen , 3 , buf , ARRAYSIZE( buf ) );

			render.GetTextSize( font_index , w , h , buf );
		}

#pragma endregion

#pragma region IBaseClientDLL

		string ClientClass_get_m_pNetworkName( ClientClass* self )
		{
			return string( self->m_pNetworkName );
		}

#pragma endregion

#pragma region IVEngineClient

		void asIVEngineClient_Con_NPrintf( asIScriptGeneric* Gen )
		{
			int pos = *(PINT)Gen->GetAddressOfArg( 0 );

			char buf[2048] = { 0 };
			asParseFormat( Gen , 1 , buf , ARRAYSIZE( buf ) );

			source::m_engine_client->Con_NPrintf( pos , buf );
		}

#pragma endregion

#pragma region ICvar

		void asICvar_ConsoleColorPrintf( asIScriptGeneric* Gen )
		{
			Color color = *(Color*)Gen->GetArgObject( 0 );

			char buf[2048] = { 0 };
			asParseFormat( Gen , 1 , buf , ARRAYSIZE( buf ) );

			source::m_cvar->ConsoleColorPrintf( color , buf );
		}

		void asICvar_ConsolePrintf( asIScriptGeneric* Gen )
		{
			char buf[2048] = { 0 };
			asParseFormat( Gen , 0 , buf , ARRAYSIZE( buf ) );

			source::m_cvar->ConsolePrintf( buf );
		}

#pragma endregion

#pragma region model_t

		string model_t_get_name( model_t* self )
		{
			return string( self->szName );
		}

#pragma endregion

#pragma region player_info_t

		string player_info_t_get_name( player_info_t* self )
		{
			return string( self->name );
		}

#pragma endregion

#pragma region ImVec2

		void ImVec2_Constructor( void* memory )
		{
			new( memory ) ImVec2();
		}

		void ImVec2_InitConstructor( float x , float y , ImVec2* self )
		{
			new( self ) ImVec2( x , y );
		}

#pragma endregion

#pragma region ImVec4

		void ImVec4_Constructor( void* memory )
		{
			new( memory ) ImVec4();
		}

		void ImVec4_InitConstructor( float x , float y , float z , float w , ImVec4* self )
		{
			new( self ) ImVec4( x , y , z , w );
		}

#pragma endregion

		void RegisterScriptAndromeda( asIScriptEngine* script_engine )
		{
			script_engine->SetDefaultNamespace( "" );

			// global typedef
			{
				script_engine->RegisterTypedef( XorStr( "byte" ) , XorStr( "uint8" ) );
				script_engine->RegisterTypedef( XorStr( "word" ) , XorStr( "uint16" ) );
				script_engine->RegisterTypedef( XorStr( "dword" ) , XorStr( "uint" ) );
				script_engine->RegisterTypedef( XorStr( "qword" ) , XorStr( "uint64" ) );

				script_engine->RegisterTypedef( XorStr( "D3DCOLOR" ) , XorStr( "uint" ) );
				script_engine->RegisterTypedef( XorStr( "ImU32" ) , XorStr( "uint" ) );
			}

			// Vector2
			{
				script_engine->RegisterObjectType( XorStr( "Vector2" ) , sizeof( Vector2 ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Vector2>() | asOBJ_APP_CLASS_ALLFLOATS );

				script_engine->RegisterObjectBehaviour( XorStr( "Vector2" ) , asBEHAVE_CONSTRUCT , XorStr( "void f()" ) , asFUNCTION( Vector2_Constructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Vector2" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(float, float)" ) , asFUNCTION( Vector2_InitConstructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Vector2" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(const Vector2 &in)" ) , asFUNCTION( Vector2_CopyConstructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Vector2" ) , asBEHAVE_LIST_CONSTRUCT , XorStr( "void f(const Vector2 &in) {float, float}" ) , asFUNCTION( Vector2_ListConstructor ) , asCALL_CDECL_OBJLAST );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "void Init(float x = 0.f,float y = 0.f)" ) , asMETHOD( Vector2 , Init ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "void Negate()" ) , asMETHOD( Vector2 , Negate ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "bool IsZero()" ) , asMETHOD( Vector2 , IsZero ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float Dot(const Vector2 &in) const" ) , asMETHOD( Vector2 , Dot ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float LengthSquared() const" ) , asMETHOD( Vector2 , LengthSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float Length() const" ) , asMETHOD( Vector2 , Length ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float DistanceSquared(const Vector2 &in) const" ) , asMETHOD( Vector2 , DistanceSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float Distance(const Vector2 &in) const" ) , asMETHOD( Vector2 , Distance ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float Normalize()" ) , asMETHOD( Vector2 , Normalize ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "void NormalizeFast()" ) , asMETHOD( Vector2 , NormalizeFast ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float& opIndex(uint idx)" ) , asMETHODPR( Vector2 , At , ( unsigned int ) , float& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "float opIndex(uint idx) const" ) , asMETHODPR( Vector2 , At , ( unsigned int ) const , const float ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opAssign(const Vector2 &in)" ) , asMETHOD( Vector2 , operator= ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "bool opEquals(const Vector2 &in) const" ) , asMETHOD( Vector2 , operator== ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opAddAssign(const Vector2 &in)" ) , asMETHODPR( Vector2 , operator+= , ( const Vector2& ) , Vector2& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opSubAssign(const Vector2 &in)" ) , asMETHODPR( Vector2 , operator-= , ( const Vector2& ) , Vector2& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opMulAssign(const Vector2 &in)" ) , asMETHODPR( Vector2 , operator*= , ( const Vector2& ) , Vector2& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opDivAssign(const Vector2 &in)" ) , asMETHODPR( Vector2 , operator/= , ( const Vector2& ) , Vector2& ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opAddAssign(float)" ) , asMETHODPR( Vector2 , operator+= , ( float ) , Vector2& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opSubAssign(float)" ) , asMETHODPR( Vector2 , operator-= , ( float ) , Vector2& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opMulAssign(float)" ) , asMETHODPR( Vector2 , operator*= , ( float ) , Vector2& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2& opDivAssign(float)" ) , asMETHODPR( Vector2 , operator/= , ( float ) , Vector2& ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opAdd(const Vector2 &in) const" ) , asMETHODPR( Vector2 , operator+ , ( const Vector2& ) const , Vector2 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opSub(const Vector2 &in) const" ) , asMETHODPR( Vector2 , operator- , ( const Vector2& ) const , Vector2 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opMul(const Vector2 &in) const" ) , asMETHODPR( Vector2 , operator* , ( const Vector2& ) const , Vector2 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opDiv(const Vector2 &in) const" ) , asMETHODPR( Vector2 , operator/ , ( const Vector2& ) const , Vector2 ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opAdd(float) const" ) , asMETHODPR( Vector2 , operator+ , ( float ) const , Vector2 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opSub(float) const" ) , asMETHODPR( Vector2 , operator- , ( float ) const , Vector2 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opMul(float) const" ) , asMETHODPR( Vector2 , operator* , ( float ) const , Vector2 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector2" ) , XorStr( "Vector2 opDiv(float) const" ) , asMETHODPR( Vector2 , operator/ , ( float ) const , Vector2 ) , asCALL_THISCALL );

				script_engine->RegisterObjectProperty( XorStr( "Vector2" ) , XorStr( "float x" ) , asOFFSET( Vector2 , m_x ) );
				script_engine->RegisterObjectProperty( XorStr( "Vector2" ) , XorStr( "float y" ) , asOFFSET( Vector2 , m_y ) );
			}

			// Vector3
			{
				script_engine->RegisterObjectType( XorStr( "Vector3" ) , sizeof( Vector3 ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Vector3>() | asOBJ_APP_CLASS_ALLFLOATS );

				script_engine->RegisterObjectBehaviour( XorStr( "Vector3" ) , asBEHAVE_CONSTRUCT , XorStr( "void f()" ) , asFUNCTION( Vector3_Constructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Vector3" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(float, float, float)" ) , asFUNCTION( Vector3_InitConstructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Vector3" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(const Vector3 &in)" ) , asFUNCTION( Vector3_CopyConstructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Vector3" ) , asBEHAVE_LIST_CONSTRUCT , XorStr( "void f(const Vector3 &in) {float, float, float}" ) , asFUNCTION( Vector3_ListConstructor ) , asCALL_CDECL_OBJLAST );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "void Init(float x = 0.f,float y = 0.f,float z = 0.f)" ) , asMETHOD( Vector3 , Init ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "bool IsZero()" ) , asMETHOD( Vector3 , IsZero ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float Dot(const Vector3 &in) const" ) , asMETHOD( Vector3 , Dot ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float LengthSquared() const" ) , asMETHOD( Vector3 , LengthSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float Length() const" ) , asMETHOD( Vector3 , Length ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float Length2DSquared() const" ) , asMETHOD( Vector3 , Length2DSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float Length2D() const" ) , asMETHOD( Vector3 , Length2D ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float DistanceSquared(const Vector3 &in) const" ) , asMETHOD( Vector3 , DistanceSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float Distance(const Vector3 &in) const" ) , asMETHOD( Vector3 , Distance ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float Normalize()" ) , asMETHOD( Vector3 , Normalize ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "void NormalizeFast()" ) , asMETHOD( Vector3 , NormalizeFast ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 Cross(const Vector3 &in)" ) , asMETHOD( Vector3 , Cross ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opAssign(const Vector3 &in)" ) , asMETHOD( Vector3 , operator= ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float& opIndex(uint idx)" ) , asMETHODPR( Vector3 , At , ( unsigned int ) , float& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "float opIndex(uint idx) const" ) , asMETHODPR( Vector3 , At , ( unsigned int ) const , const float ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "bool opEquals(const Vector3 &in) const" ) , asMETHOD( Vector3 , operator== ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opAddAssign(const Vector3 &in)" ) , asMETHODPR( Vector3 , operator+= , ( const Vector3& ) , Vector3& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opSubAssign(const Vector3 &in)" ) , asMETHODPR( Vector3 , operator-= , ( const Vector3& ) , Vector3& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opMulAssign(const Vector3 &in)" ) , asMETHODPR( Vector3 , operator*= , ( const Vector3& ) , Vector3& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opDivAssign(const Vector3 &in)" ) , asMETHODPR( Vector3 , operator/= , ( const Vector3& ) , Vector3& ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opAddAssign(float)" ) , asMETHODPR( Vector3 , operator+= , ( float ) , Vector3& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opSubAssign(float)" ) , asMETHODPR( Vector3 , operator-= , ( float ) , Vector3& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opMulAssign(float)" ) , asMETHODPR( Vector3 , operator*= , ( float ) , Vector3& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3& opDivAssign(float)" ) , asMETHODPR( Vector3 , operator/= , ( float ) , Vector3& ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opAdd(const Vector3 &in) const" ) , asMETHODPR( Vector3 , operator+ , ( const Vector3& ) const , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opSub(const Vector3 &in) const" ) , asMETHODPR( Vector3 , operator- , ( const Vector3& ) const , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opMul(const Vector3 &in) const" ) , asMETHODPR( Vector3 , operator* , ( const Vector3& ) const , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opDiv(const Vector3 &in) const" ) , asMETHODPR( Vector3 , operator/ , ( const Vector3& ) const , Vector3 ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opAdd(float) const" ) , asMETHODPR( Vector3 , operator+ , ( float ) const , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opSub(float) const" ) , asMETHODPR( Vector3 , operator- , ( float ) const , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opMul(float) const" ) , asMETHODPR( Vector3 , operator* , ( float ) const , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "Vector3" ) , XorStr( "Vector3 opDiv(float) const" ) , asMETHODPR( Vector3 , operator/ , ( float ) const , Vector3 ) , asCALL_THISCALL );

				script_engine->RegisterObjectProperty( XorStr( "Vector3" ) , XorStr( "float x" ) , asOFFSET( Vector3 , m_x ) );
				script_engine->RegisterObjectProperty( XorStr( "Vector3" ) , XorStr( "float y" ) , asOFFSET( Vector3 , m_y ) );
				script_engine->RegisterObjectProperty( XorStr( "Vector3" ) , XorStr( "float z" ) , asOFFSET( Vector3 , m_z ) );
			}

			// QAngle
			{
				script_engine->RegisterObjectType( XorStr( "QAngle" ) , sizeof( QAngle ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<QAngle>() | asOBJ_APP_CLASS_ALLFLOATS );

				script_engine->RegisterObjectBehaviour( XorStr( "QAngle" ) , asBEHAVE_CONSTRUCT , XorStr( "void f()" ) , asFUNCTION( QAngle_Constructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "QAngle" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(float, float, float)" ) , asFUNCTION( QAngle_InitConstructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "QAngle" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(const QAngle &in)" ) , asFUNCTION( QAngle_CopyConstructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "QAngle" ) , asBEHAVE_LIST_CONSTRUCT , XorStr( "void f(const QAngle &in) {float, float, float}" ) , asFUNCTION( QAngle_ListConstructor ) , asCALL_CDECL_OBJLAST );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "void Init(float x = 0.f,float y = 0.f,float z = 0.f)" ) , asMETHOD( QAngle , Init ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "void Negate()" ) , asMETHOD( QAngle , Negate ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "bool IsZero()" ) , asMETHOD( QAngle , IsZero ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float Dot(const QAngle &in) const" ) , asMETHOD( QAngle , Dot ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float LengthSquared() const" ) , asMETHOD( QAngle , LengthSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float Length() const" ) , asMETHOD( QAngle , Length ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float DistanceSquared(const QAngle &in) const" ) , asMETHOD( QAngle , DistanceSquared ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float Distance(const QAngle &in) const" ) , asMETHOD( QAngle , Distance ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "void Normalize()" ) , asMETHOD( QAngle , Normalize ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "void Clamp()" ) , asMETHOD( QAngle , Clamp ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opAssign(const QAngle &in)" ) , asMETHOD( QAngle , operator= ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float& opIndex(uint idx)" ) , asMETHODPR( QAngle , At , ( unsigned int ) , float& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "float opIndex(uint idx) const" ) , asMETHODPR( QAngle , At , ( unsigned int ) const , const float ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "bool opEquals(const QAngle &in) const" ) , asMETHOD( QAngle , operator== ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opAddAssign(const QAngle &in)" ) , asMETHODPR( QAngle , operator+= , ( const QAngle& ) , QAngle& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opSubAssign(const QAngle &in)" ) , asMETHODPR( QAngle , operator-= , ( const QAngle& ) , QAngle& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opMulAssign(const QAngle &in)" ) , asMETHODPR( QAngle , operator*= , ( const QAngle& ) , QAngle& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opDivAssign(const QAngle &in)" ) , asMETHODPR( QAngle , operator/= , ( const QAngle& ) , QAngle& ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opAddAssign(float)" ) , asMETHODPR( QAngle , operator+= , ( float ) , QAngle& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opSubAssign(float)" ) , asMETHODPR( QAngle , operator-= , ( float ) , QAngle& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opMulAssign(float)" ) , asMETHODPR( QAngle , operator*= , ( float ) , QAngle& ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle& opDivAssign(float)" ) , asMETHODPR( QAngle , operator/= , ( float ) , QAngle& ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opAdd(const QAngle &in) const" ) , asMETHODPR( QAngle , operator+ , ( const QAngle& ) const , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opSub(const QAngle &in) const" ) , asMETHODPR( QAngle , operator- , ( const QAngle& ) const , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opMul(const QAngle &in) const" ) , asMETHODPR( QAngle , operator* , ( const QAngle& ) const , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opDiv(const QAngle &in) const" ) , asMETHODPR( QAngle , operator/ , ( const QAngle& ) const , QAngle ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opAdd(float) const" ) , asMETHODPR( QAngle , operator+ , ( float ) const , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opSub(float) const" ) , asMETHODPR( QAngle , operator- , ( float ) const , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opMul(float) const" ) , asMETHODPR( QAngle , operator* , ( float ) const , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "QAngle" ) , XorStr( "QAngle opDiv(float) const" ) , asMETHODPR( QAngle , operator/ , ( float ) const , QAngle ) , asCALL_THISCALL );

				script_engine->RegisterObjectProperty( XorStr( "QAngle" ) , XorStr( "float x" ) , asOFFSET( QAngle , m_x ) );
				script_engine->RegisterObjectProperty( XorStr( "QAngle" ) , XorStr( "float y" ) , asOFFSET( QAngle , m_y ) );
				script_engine->RegisterObjectProperty( XorStr( "QAngle" ) , XorStr( "float z" ) , asOFFSET( QAngle , m_z ) );
			}

			// Color
			{
				script_engine->RegisterObjectType( XorStr( "Color" ) , sizeof( Color ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Color>() | asOBJ_APP_CLASS_ALLFLOATS );

				script_engine->RegisterObjectBehaviour( XorStr( "Color" ) , asBEHAVE_CONSTRUCT , XorStr( "void f()" ) , asFUNCTION( Color_Constructor ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Color" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(uint8, uint8, uint8, uint8 a = 255)" ) , asFUNCTIONPR( Color_InitConstructor , ( BYTE , BYTE , BYTE , BYTE , Color* ) , void ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Color" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(uint)" ) , asFUNCTIONPR( Color_InitConstructor , ( D3DCOLOR , Color* ) , void ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectBehaviour( XorStr( "Color" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(const Color &in)" ) , asFUNCTION( Color_CopyConstructor ) , asCALL_CDECL_OBJLAST );

				script_engine->RegisterObjectProperty( XorStr( "Color" ) , XorStr( "uint8 r" ) , asOFFSET( Color , m_r ) );
				script_engine->RegisterObjectProperty( XorStr( "Color" ) , XorStr( "uint8 g" ) , asOFFSET( Color , m_g ) );
				script_engine->RegisterObjectProperty( XorStr( "Color" ) , XorStr( "uint8 b" ) , asOFFSET( Color , m_b ) );
				script_engine->RegisterObjectProperty( XorStr( "Color" ) , XorStr( "uint8 a" ) , asOFFSET( Color , m_a ) );
			}

			// render
			{
				script_engine->SetDefaultNamespace( XorStr( "Render" ) );

				auto& render = Render::Instance();

				script_engine->RegisterGlobalFunction( XorStr( "uint CreateFont(string &in, int, uint)" ) , asMETHOD( Render , CreateFont ) , asCALL_THISCALL_ASGLOBAL , &render );
				
				script_engine->RegisterGlobalFunction( XorStr( "void Line(int, int, int, int, Color)" ) , asMETHODPR( Render , Line , ( int , int , int , int , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void Line(int, int, Vector2, Color)" ) , asMETHODPR( Render , Line , ( int , int , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void Line(Vector2, Vector2, Color)" ) , asMETHODPR( Render , Line , ( Vector2 , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				
				script_engine->RegisterGlobalFunction( XorStr( "void Box(int, int, int, int, Color)" ) , asMETHODPR( Render , Box , ( int , int , int , int , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void Box(int, int, Vector2, Color)" ) , asMETHODPR( Render , Box , ( int , int , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void Box(Vector2, Vector2, Color)" ) , asMETHODPR( Render , Box , ( Vector2 , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );

				script_engine->RegisterGlobalFunction( XorStr( "void FillBox(int, int, int, int, Color)" ) , asMETHODPR( Render , FillBox , ( int , int , int , int , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void FillBox(int, int, Vector2, Color)" ) , asMETHODPR( Render , FillBox , ( int , int , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void FillBox(Vector2, Vector2, Color)" ) , asMETHODPR( Render , FillBox , ( Vector2 , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				
				script_engine->RegisterGlobalFunction( XorStr( "void Circle(int, int, int, Color)" ) , asMETHODPR( Render , Circle , ( int , int , int , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void Circle(Vector2, int, Color)" ) , asMETHODPR( Render , Circle , ( Vector2 , int , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				
				script_engine->RegisterGlobalFunction( XorStr( "void OutlineBox(int, int, int, int, Color)" ) , asMETHODPR( Render , OutlineBox , ( int , int , int , int , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void OutlineBox(int, int, Vector2, Color)" ) , asMETHODPR( Render , OutlineBox , ( int , int , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				script_engine->RegisterGlobalFunction( XorStr( "void OutlineBox(Vector2, Vector2, Color)" ) , asMETHODPR( Render , OutlineBox , ( Vector2 , Vector2 , Color ) , void ) , asCALL_THISCALL_ASGLOBAL , &render );
				
				script_engine->RegisterGlobalFunction( XorStr( "void Text(uint, int, int, uint, Color, string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asDrawText ) , asCALL_GENERIC );
				script_engine->RegisterGlobalFunction( XorStr( "void GetTextSize(uint, int &out, int &out, string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asGetTextSize ) , asCALL_GENERIC );

				script_engine->SetDefaultNamespace( "" );
			}

			// ClientClass
			{
				script_engine->RegisterObjectType( XorStr( "ClientClass" ) , 0 , asOBJ_REF | asOBJ_NOCOUNT );
				script_engine->RegisterObjectMethod( XorStr( "ClientClass" ) , XorStr( "string get_m_pNetworkName()" ) , asFUNCTION( ClientClass_get_m_pNetworkName ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectProperty( XorStr( "ClientClass" ) , XorStr( "ClientClass@ m_pNext" ) , asOFFSET( ClientClass , m_pNext ) );
				script_engine->RegisterObjectProperty( XorStr( "ClientClass" ) , XorStr( "int m_ClassID" ) , asOFFSET( ClientClass , m_ClassID ) );
			}

			// model_t
			{
				script_engine->RegisterObjectType( XorStr( "model_t" ) , sizeof( model_t ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<model_t>() );

				script_engine->RegisterObjectMethod( XorStr( "model_t" ) , XorStr( "const string get_name()" ) , asFUNCTION( model_t_get_name ) , asCALL_CDECL_OBJLAST );

				script_engine->RegisterObjectProperty( XorStr( "model_t" ) , XorStr( "const Vector3 vecMins" ) , asOFFSET( model_t , vecMins ) );
				script_engine->RegisterObjectProperty( XorStr( "model_t" ) , XorStr( "const Vector3 vecMaxs" ) , asOFFSET( model_t , vecMaxs ) );
			}

			// IBaseClientDLL
			{
				script_engine->RegisterObjectType( XorStr( "IBaseClientDLL" ) , 0 , asOBJ_REF | asOBJ_NOHANDLE );
				script_engine->RegisterObjectMethod( XorStr( "IBaseClientDLL" ) , XorStr( "ClientClass@ GetAllClasses()" ) , asMETHOD( IBaseClientDLL , GetAllClasses ) , asCALL_THISCALL );

				script_engine->SetDefaultNamespace( XorStr( "source" ) );
				script_engine->RegisterGlobalProperty( XorStr( "IBaseClientDLL m_base_client" ) , source::m_base_client );
				script_engine->SetDefaultNamespace( "" );
			}

			// IClientEntity
			{
				script_engine->RegisterObjectType( XorStr( "IClientEntity" ) , 0 , asOBJ_REF | asOBJ_NOCOUNT );

				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "int GetPropInt(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<int> , ( string , string , DWORD ) , int ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "void SetPropInt(int,string,string)" ) , asMETHODPR( IClientEntity , SetNetProp<int> , ( int , string , string ) , void ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "bool GetPropBool(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<bool> , ( string , string , DWORD ) , bool ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "void SetPropBool(bool,string,string)" ) , asMETHODPR( IClientEntity , SetNetProp<bool> , ( bool , string , string ) , void ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "byte GetPropByte(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<BYTE> , ( string , string , DWORD ) , BYTE ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "word GetPropWord(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<asWORD> , ( string , string , DWORD ) , asWORD ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "dword GetPropDword(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<asDWORD> , ( string , string , DWORD ) , asDWORD ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "float GetPropFloat(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<float> , ( string , string , DWORD ) , float ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "void SetPropFloat(float,string,string)" ) , asMETHODPR( IClientEntity , SetNetProp<float> , ( float , string , string ) , void ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "Vector3 GetPropVector3(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<Vector3> , ( string , string , DWORD ) , Vector3 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "void SetPropVector3(Vector3,string,string)" ) , asMETHODPR( IClientEntity , SetNetProp<Vector3> , ( Vector3 , string , string ) , void ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "QAngle GetPropQAngle(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetProp<QAngle> , ( string , string , DWORD ) , QAngle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "void SetPropQAngle(QAngle,string,string)" ) , asMETHODPR( IClientEntity , SetNetProp<QAngle> , ( QAngle , string , string ) , void ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "string GetPropString(string,string,uint def = 0)" ) , asMETHODPR( IClientEntity , GetNetPropString , ( string , string , DWORD ) , string ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "Vector3& OBBMins() const" ) , asMETHOD( IClientEntity , OBBMins ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "Vector3& OBBMaxs() const" ) , asMETHOD( IClientEntity , OBBMaxs ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "ClientClass@ GetClientClass()" ) , asMETHOD( IClientEntity , GetClientClass ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "bool IsDormant()" ) , asMETHOD( IClientEntity , IsDormant ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "int entindex()" ) , asMETHOD( IClientEntity , entindex ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "model_t& GetModel() const" ) , asMETHOD( IClientEntity , GetModel ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "Vector3& GetAbsOrigin() const" ) , asMETHOD( IClientEntity , GetAbsOrigin ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntity" ) , XorStr( "QAngle& GetAbsAngles() const" ) , asMETHOD( IClientEntity , GetAbsAngles ) , asCALL_THISCALL );
			}

			// IClientEntityList
			{
				script_engine->RegisterObjectType( XorStr( "IClientEntityList" ) , 0 , asOBJ_REF | asOBJ_NOHANDLE );

				script_engine->RegisterObjectMethod( XorStr( "IClientEntityList" ) , XorStr( "IClientEntity@ GetClientEntity(int)" ) , asMETHOD( IClientEntityList , GetClientEntity ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntityList" ) , XorStr( "IClientEntity@ GetClientEntityFromHandle(uint)" ) , asMETHOD( IClientEntityList , GetClientEntityFromHandle ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IClientEntityList" ) , XorStr( "int GetHighestEntityIndex()" ) , asMETHOD( IClientEntityList , GetHighestEntityIndex ) , asCALL_THISCALL );

				script_engine->SetDefaultNamespace( XorStr( "source" ) );
				script_engine->RegisterGlobalProperty( XorStr( "IClientEntityList m_client_entity_list" ) , source::m_client_entity_list );
				script_engine->SetDefaultNamespace( "" );
			}
			
			// player_info_t
			{
				script_engine->RegisterObjectType( XorStr( "player_info_t" ) , sizeof( player_info_t ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<player_info_t>() );

				script_engine->RegisterObjectProperty( XorStr( "player_info_t" ) , XorStr( "const int64 steamID64" ) , asOFFSET( player_info_t , steamID64 ) );
				script_engine->RegisterObjectMethod( XorStr( "player_info_t" ) , XorStr( "const string get_name()" ) , asFUNCTION( player_info_t_get_name ) , asCALL_CDECL_OBJLAST );
				script_engine->RegisterObjectProperty( XorStr( "player_info_t" ) , XorStr( "const int userid" ) , asOFFSET( player_info_t , userid ) );
				script_engine->RegisterObjectProperty( XorStr( "player_info_t" ) , XorStr( "const bool fakeplayer" ) , asOFFSET( player_info_t , fakeplayer ) );
				script_engine->RegisterObjectProperty( XorStr( "player_info_t" ) , XorStr( "const bool ishltv" ) , asOFFSET( player_info_t , ishltv ) );
			}

			// IVEngineClient
			{
				script_engine->RegisterObjectType( XorStr( "IVEngineClient" ) , 0 , asOBJ_REF | asOBJ_NOHANDLE );

				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "void GetScreenSize(int &out,int &out)" ) , asMETHOD( IVEngineClient , GetScreenSize ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "void GetPlayerInfo(int,player_info_t &out)" ) , asMETHOD( IVEngineClient , GetPlayerInfo ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "int GetPlayerForUserID(int)" ) , asMETHOD( IVEngineClient , GetPlayerForUserID ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "int GetLocalPlayer()" ) , asMETHOD( IVEngineClient , GetLocalPlayer ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "float GetLastTimeStamp()" ) , asMETHOD( IVEngineClient , GetLastTimeStamp ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "void GetViewAngles(QAngle &in)" ) , asMETHOD( IVEngineClient , GetViewAngles ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "void SetViewAngles(QAngle &in)" ) , asMETHOD( IVEngineClient , SetViewAngles ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "int GetMaxClients()" ) , asMETHOD( IVEngineClient , GetMaxClients ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "bool IsInGame()" ) , asMETHOD( IVEngineClient , IsInGame ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "void Con_NPrintf(int,string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asIVEngineClient_Con_NPrintf ) , asCALL_GENERIC );	
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "bool Con_IsVisible()" ) , asMETHOD( IVEngineClient , Con_IsVisible ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "string GetGameDirectory()" ) , asMETHOD( IVEngineClient , GetGameDirectory ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "string GetLevelNameShort()" ) , asMETHOD( IVEngineClient , GetLevelNameShort ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "void ClientCmd(string)" ) , asMETHOD( IVEngineClient , ClientCmd_Unrestricted ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IVEngineClient" ) , XorStr( "bool IsActiveApp()" ) , asMETHOD( IVEngineClient , IsActiveApp ) , asCALL_THISCALL );

				script_engine->SetDefaultNamespace( XorStr( "source" ) );
				script_engine->RegisterGlobalProperty( XorStr( "IVEngineClient m_engine_client" ) , source::m_engine_client );
				script_engine->SetDefaultNamespace( "" );
			}

			// IGameEvent
			{
				script_engine->RegisterObjectType( XorStr( "IGameEvent" ) , 0 , asOBJ_REF | asOBJ_NOCOUNT );

				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "string GetName()" ) , asMETHOD( IGameEvent , GetName ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "bool IsReliable()" ) , asMETHOD( IGameEvent , IsReliable ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "bool IsLocal()" ) , asMETHOD( IGameEvent , IsLocal ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "bool IsEmpty(string)" ) , asMETHOD( IGameEvent , IsEmpty ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "bool GetBool(string,bool def = false)" ) , asMETHOD( IGameEvent , GetBool ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "int GetInt(string,int def = 0)" ) , asMETHOD( IGameEvent , GetInt ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "uint64 GetUint64(string,uint64 def = 0)" ) , asMETHOD( IGameEvent , GetUint64 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "float GetFloat(string,float def = 0.f)" ) , asMETHOD( IGameEvent , GetFloat ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "string GetString(string,string def = \"\")" ) , asMETHOD( IGameEvent , GetString ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "void SetBool(string,bool def = false)" ) , asMETHOD( IGameEvent , SetBool ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "void SetInt(string,int def = 0)" ) , asMETHOD( IGameEvent , SetInt ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "void SetUint64(string,uint64 def = 0)" ) , asMETHOD( IGameEvent , SetUint64 ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "void SetFloat(string,float def = 0.f)" ) , asMETHOD( IGameEvent , SetFloat ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IGameEvent" ) , XorStr( "void SetString(string,string def = \"\")" ) , asMETHOD( IGameEvent , SetString ) , asCALL_THISCALL );
			}

			// CGlobalVarsBase
			{
				script_engine->RegisterObjectType( XorStr( "CGlobalVarsBase" ) , 0 , asOBJ_REF | asOBJ_NOHANDLE );

				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float realtime" ) , asOFFSET( CGlobalVarsBase , realtime ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const int framecount" ) , asOFFSET( CGlobalVarsBase , framecount ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float absoluteframetime" ) , asOFFSET( CGlobalVarsBase , absoluteframetime ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float absoluteframestarttimestddev" ) , asOFFSET( CGlobalVarsBase , absoluteframestarttimestddev ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float curtime" ) , asOFFSET( CGlobalVarsBase , curtime ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float frametime" ) , asOFFSET( CGlobalVarsBase , frametime ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const int maxClients" ) , asOFFSET( CGlobalVarsBase , maxClients ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const int tickcount" ) , asOFFSET( CGlobalVarsBase , tickcount ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float interval_per_tick" ) , asOFFSET( CGlobalVarsBase , interval_per_tick ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const float interpolation_amount" ) , asOFFSET( CGlobalVarsBase , interpolation_amount ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const int simTicksThisFrame" ) , asOFFSET( CGlobalVarsBase , simTicksThisFrame ) );
				script_engine->RegisterObjectProperty( XorStr( "CGlobalVarsBase" ) , XorStr( "const int network_protocol" ) , asOFFSET( CGlobalVarsBase , network_protocol ) );

				script_engine->SetDefaultNamespace( XorStr( "source" ) );
				script_engine->RegisterGlobalProperty( XorStr( "CGlobalVarsBase m_globals" ) , source::m_globals );
				script_engine->SetDefaultNamespace( "" );
			}

			// ConVar
			{
				script_engine->RegisterObjectType( XorStr( "ConVar" ) , 0 , asOBJ_REF | asOBJ_NOCOUNT );

				script_engine->RegisterObjectMethod( XorStr( "ConVar" ) , XorStr( "float GetFloat()" ) , asMETHOD( ConVar , GetFloat ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "ConVar" ) , XorStr( "int GetInt()" ) , asMETHOD( ConVar , GetInt ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "ConVar" ) , XorStr( "bool GetBool()" ) , asMETHOD( ConVar , GetBool ) , asCALL_THISCALL );

				script_engine->RegisterObjectMethod( XorStr( "ConVar" ) , XorStr( "void SetValue(float)" ) , asMETHODPR( ConVar , SetValue , ( float ) , void ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "ConVar" ) , XorStr( "void SetValue(int)" ) , asMETHODPR( ConVar , SetValue , ( int ) , void ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "ConVar" ) , XorStr( "void SetValue(bool)" ) , asMETHODPR( ConVar , SetValue , ( bool ) , void ) , asCALL_THISCALL );
			}

			// ICvar
			{
				script_engine->RegisterObjectType( XorStr( "ICvar" ) , 0 , asOBJ_REF | asOBJ_NOHANDLE );

				script_engine->RegisterObjectMethod( XorStr( "ICvar" ) , XorStr( "ConVar@ FindVar(string)" ) , asMETHOD( ICvar , FindVar ) , asCALL_THISCALL );
				
				script_engine->RegisterObjectMethod( XorStr( "ICvar" ) , XorStr( "void ConsoleColorPrintf(Color,string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asICvar_ConsoleColorPrintf ) , asCALL_GENERIC );
				script_engine->RegisterObjectMethod( XorStr( "ICvar" ) , XorStr( "void ConsolePrintf(string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asICvar_ConsolePrintf ) , asCALL_GENERIC );

				script_engine->SetDefaultNamespace( XorStr( "source" ) );
				script_engine->RegisterGlobalProperty( XorStr( "ICvar m_cvar" ) , source::m_cvar );
				script_engine->SetDefaultNamespace( "" );
			}

			// CUserCmd
			{
				script_engine->RegisterObjectType( XorStr( "CUserCmd" ) , 0 , asOBJ_REF | asOBJ_NOCOUNT );

				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "int command_number" ) , asOFFSET( CUserCmd , command_number ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "int tick_count" ) , asOFFSET( CUserCmd , tick_count ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "QAngle viewangles" ) , asOFFSET( CUserCmd , viewangles ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "Vector3 aimdirection" ) , asOFFSET( CUserCmd , aimdirection ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "float forwardmove" ) , asOFFSET( CUserCmd , forwardmove ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "float sidemove" ) , asOFFSET( CUserCmd , sidemove ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "float upmove" ) , asOFFSET( CUserCmd , upmove ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "int buttons" ) , asOFFSET( CUserCmd , buttons ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "int random_seed" ) , asOFFSET( CUserCmd , random_seed ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "int16 mousedx" ) , asOFFSET( CUserCmd , mousedx ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "int16 mousedy" ) , asOFFSET( CUserCmd , mousedy ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "QAngle headangles" ) , asOFFSET( CUserCmd , headangles ) );
				script_engine->RegisterObjectProperty( XorStr( "CUserCmd" ) , XorStr( "Vector3 headoffset" ) , asOFFSET( CUserCmd , headoffset ) );
			}

			// IInputSystem
			{
				script_engine->RegisterObjectType( XorStr( "IInputSystem" ) , 0 , asOBJ_REF | asOBJ_NOHANDLE );

				script_engine->RegisterObjectMethod( XorStr( "IInputSystem" ) , XorStr( "void EnableInput(bool)" ) , asMETHOD( IInputSystem , EnableInput ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IInputSystem" ) , XorStr( "bool IsButtonDown(int)" ) , asMETHOD( IInputSystem , IsButtonDown ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IInputSystem" ) , XorStr( "string ButtonCodeToString(int)" ) , asMETHOD( IInputSystem , ButtonCodeToString ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IInputSystem" ) , XorStr( "int StringToButtonCode(string)" ) , asMETHOD( IInputSystem , StringToButtonCode ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IInputSystem" ) , XorStr( "int VirtualKeyToButtonCode(int)" ) , asMETHOD( IInputSystem , VirtualKeyToButtonCode ) , asCALL_THISCALL );
				script_engine->RegisterObjectMethod( XorStr( "IInputSystem" ) , XorStr( "int ButtonCodeToVirtualKey(int)" ) , asMETHOD( IInputSystem , ButtonCodeToVirtualKey ) , asCALL_THISCALL );

				script_engine->SetDefaultNamespace( XorStr( "source" ) );
				script_engine->RegisterGlobalProperty( XorStr( "IInputSystem m_input_system" ) , source::m_input_system );
				script_engine->SetDefaultNamespace( "" );
			}

			// cheat
			{
				// imgui
				{
					// ImVec2
					{				
						script_engine->RegisterObjectType( XorStr( "ImVec2" ) , sizeof( ImVec2 ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ImVec2>() | asOBJ_APP_CLASS_ALLFLOATS );

						script_engine->RegisterObjectBehaviour( XorStr( "ImVec2" ) , asBEHAVE_CONSTRUCT , XorStr( "void f()" ) , asFUNCTION( ImVec2_Constructor ) , asCALL_CDECL_OBJLAST );
						script_engine->RegisterObjectBehaviour( XorStr( "ImVec2" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(float, float)" ) , asFUNCTION( ImVec2_InitConstructor ) , asCALL_CDECL_OBJLAST );

						script_engine->RegisterObjectMethod( XorStr( "ImVec2" ) , XorStr( "float opIndex(uint idx) const" ) , asMETHODPR( ImVec2 , operator[] , ( size_t ) const , float ) , asCALL_THISCALL );
						script_engine->RegisterObjectMethod( XorStr( "ImVec2" ) , XorStr( "float& opIndex(uint idx)" ) , asMETHODPR( ImVec2 , operator[] , ( size_t ) , float& ) , asCALL_THISCALL );

						script_engine->RegisterObjectProperty( XorStr( "ImVec2" ) , XorStr( "float x" ) , asOFFSET( ImVec2 , x ) );
						script_engine->RegisterObjectProperty( XorStr( "ImVec2" ) , XorStr( "float y" ) , asOFFSET( ImVec2 , y ) );
					}

					// ImVec4
					{
						script_engine->RegisterObjectType( XorStr( "ImVec4" ) , sizeof( ImVec4 ) , asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ImVec4>() | asOBJ_APP_CLASS_ALLFLOATS );

						script_engine->RegisterObjectBehaviour( XorStr( "ImVec4" ) , asBEHAVE_CONSTRUCT , XorStr( "void f()" ) , asFUNCTION( ImVec4_Constructor ) , asCALL_CDECL_OBJLAST );
						script_engine->RegisterObjectBehaviour( XorStr( "ImVec4" ) , asBEHAVE_CONSTRUCT , XorStr( "void f(float, float, float, float)" ) , asFUNCTION( ImVec4_InitConstructor ) , asCALL_CDECL_OBJLAST );

						script_engine->RegisterObjectProperty( XorStr( "ImVec4" ) , XorStr( "float x" ) , asOFFSET( ImVec4 , x ) );
						script_engine->RegisterObjectProperty( XorStr( "ImVec4" ) , XorStr( "float y" ) , asOFFSET( ImVec4 , y ) );
						script_engine->RegisterObjectProperty( XorStr( "ImVec4" ) , XorStr( "float z" ) , asOFFSET( ImVec4 , z ) );
						script_engine->RegisterObjectProperty( XorStr( "ImVec4" ) , XorStr( "float w" ) , asOFFSET( ImVec4 , w ) );
					}

					auto& style = ImGui::GetStyle();
					auto& colors = style.Colors;

					// ImGuiStyle
					{
						script_engine->RegisterObjectType( XorStr( "ImGuiStyle" ) , 0 , asOBJ_REF | asOBJ_NOCOUNT );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float WindowBorderSize" ) , asOFFSET( ImGuiStyle , WindowBorderSize ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float FrameBorderSize" ) , asOFFSET( ImGuiStyle , FrameBorderSize ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec2 FramePadding" ) , asOFFSET( ImGuiStyle , FramePadding ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec2 ItemSpacing" ) , asOFFSET( ImGuiStyle , ItemSpacing ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec2 ItemInnerSpacing" ) , asOFFSET( ImGuiStyle , ItemInnerSpacing ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float Alpha" ) , asOFFSET( ImGuiStyle , Alpha ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float WindowRounding" ) , asOFFSET( ImGuiStyle , WindowRounding ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float FrameRounding" ) , asOFFSET( ImGuiStyle , FrameRounding ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float PopupRounding" ) , asOFFSET( ImGuiStyle , PopupRounding ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float PopupBorderSize" ) , asOFFSET( ImGuiStyle , PopupBorderSize ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float IndentSpacing" ) , asOFFSET( ImGuiStyle , IndentSpacing ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float GrabMinSize" ) , asOFFSET( ImGuiStyle , GrabMinSize ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float GrabRounding" ) , asOFFSET( ImGuiStyle , GrabRounding ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float ScrollbarSize" ) , asOFFSET( ImGuiStyle , ScrollbarSize ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "float ScrollbarRounding" ) , asOFFSET( ImGuiStyle , ScrollbarRounding ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_Text" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_Text] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TextDisabled" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TextDisabled] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_WindowBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_WindowBg] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ChildBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ChildBg] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_PopupBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_PopupBg] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_Border" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_Border] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_BorderShadow" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_BorderShadow] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_FrameBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_FrameBg] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_FrameBgHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_FrameBgHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_FrameBgActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_FrameBgActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TitleBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TitleBg] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TitleBgActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TitleBgActive] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TitleBgCollapsed" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TitleBgCollapsed] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_MenuBarBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_MenuBarBg] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ScrollbarBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ScrollbarBg] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ScrollbarGrab" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ScrollbarGrab] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ScrollbarGrabHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ScrollbarGrabHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ScrollbarGrabActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ScrollbarGrabActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_CheckMark" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_CheckMark] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_SliderGrab" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_SliderGrab] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_SliderGrabActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_SliderGrabActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_Button" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_Button] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ButtonHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ButtonHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ButtonActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ButtonActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_Header" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_Header] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_HeaderHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_HeaderHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_HeaderActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_HeaderActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_Separator" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_Separator] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_SeparatorHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_SeparatorHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_SeparatorActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_SeparatorActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ResizeGrip" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ResizeGrip] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ResizeGripHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ResizeGripHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ResizeGripActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ResizeGripActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_Tab" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_Tab] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TabHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TabHovered] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TabActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TabActive] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TabUnfocused" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TabUnfocused] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TabUnfocusedActive" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TabUnfocusedActive] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_PlotLines" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_PlotLines] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_PlotLinesHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_PlotLinesHovered] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_PlotHistogram" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_PlotHistogram] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_PlotHistogramHovered" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_PlotHistogramHovered] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_TextSelectedBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_TextSelectedBg] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_DragDropTarget" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_DragDropTarget] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_NavHighlight" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_NavHighlight] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_NavWindowingHighlight" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_NavWindowingHighlight] ) );
						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_NavWindowingDimBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_NavWindowingDimBg] ) );

						script_engine->RegisterObjectProperty( XorStr( "ImGuiStyle" ) , XorStr( "ImVec4 ImGuiCol_ModalWindowDimBg" ) , asOFFSET( ImGuiStyle , Colors[ImGuiCol_ModalWindowDimBg] ) );
					}

					script_engine->SetDefaultNamespace( XorStr( "ImGui" ) );

					script_engine->RegisterGlobalFunction( XorStr( "ImGuiStyle@ GetStyle()" ) , asFUNCTION( ImGui::GetStyle ) , asCALL_CDECL );

					script_engine->RegisterGlobalFunction( XorStr( "ImVec4 ColorConvertU32ToFloat4(uint)" ) , asFUNCTION( ImGui::ColorConvertU32ToFloat4 ) , asCALL_CDECL );
					script_engine->RegisterGlobalFunction( XorStr( "uint ColorConvertFloat4ToU32(ImVec4)" ) , asFUNCTION( ImGui::ColorConvertFloat4ToU32 ) , asCALL_CDECL );
					
					script_engine->RegisterGlobalFunction( XorStr( "void ColorConvertRGBtoHSV(float,float,float,float &out,float &out,float &out)" ) , asFUNCTION( ImGui::ColorConvertRGBtoHSV ) , asCALL_CDECL );
					script_engine->RegisterGlobalFunction( XorStr( "void ColorConvertHSVtoRGB(float,float,float,float &out,float &out,float &out)" ) , asFUNCTION( ImGui::ColorConvertHSVtoRGB ) , asCALL_CDECL );

					script_engine->SetDefaultNamespace( "" );
				}
			}

			// Global Function
			{
				script_engine->RegisterGlobalFunction( XorStr( "string sprintf(string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asSprintf ) , asCALL_GENERIC );
				script_engine->RegisterGlobalFunction( XorStr( "void log(string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asLog ) , asCALL_GENERIC );

				script_engine->RegisterGlobalFunction( XorStr( "string GetModuleName()" ) , asFUNCTION( asGetModuleName ) , asCALL_CDECL );
				script_engine->RegisterGlobalFunction( XorStr( "string GetScriptDir()" ) , asFUNCTION( asGetScriptDir ) , asCALL_CDECL );
				script_engine->RegisterGlobalFunction( XorStr( "void PlaySound(string)" ) , asFUNCTION( asPlaySound ) , asCALL_CDECL );

				script_engine->RegisterGlobalFunction( XorStr( "void LoadConfig(string,bool notifi = true)" ) , asFUNCTION( LoadConfig ) , asCALL_CDECL );
				script_engine->RegisterGlobalFunction( XorStr( "void SaveConfig(string,bool notifi = true)" ) , asFUNCTION( SaveConfig ) , asCALL_CDECL );

				script_engine->RegisterGlobalFunction( XorStr( "uint GetTickCount()" ) , asFUNCTION( GetTickCount ) , asCALL_CDECL );
				script_engine->RegisterGlobalFunction( XorStr( "uint64 GetTickCount64()" ) , asFUNCTION( GetTickCount64 ) , asCALL_CDECL );

				script_engine->RegisterGlobalFunction( XorStr( "void AddNotification(uint,uint,string &in, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null, ?&in = null)" ) , asFUNCTION( asAddNotification ) , asCALL_GENERIC );

				script_engine->RegisterGlobalFunction( XorStr( "bool WorldToScreen(const Vector3 &in,Vector2 &out)" ) , asFUNCTIONPR( WorldToScreen , ( const Vector3& , Vector2& ) , bool ) , asCALL_CDECL );
				script_engine->RegisterGlobalFunction( XorStr( "bool WorldToScreen(const Vector3 &in,int &out,int &out)" ) , asFUNCTIONPR( WorldToScreen , ( const Vector3& , int& , int& ) , bool ) , asCALL_CDECL );
			}
		}
	}
}