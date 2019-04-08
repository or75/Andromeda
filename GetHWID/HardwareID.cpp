#include "HardwareID.h"

namespace HardwareID
{
	vector<string> GetWmiInfo( char* szQuery , char* szPropertie , bool usb_drive )
	{
		vector<string> ReturnData;

		HRESULT hres;

		hres = CoInitializeEx( 0 , COINIT_MULTITHREADED );

		if ( FAILED( hres ) )
		{
			CoUninitialize();
			return ReturnData;
		}

		hres = CoInitializeSecurity( NULL , -1 , NULL , NULL , RPC_C_AUTHN_LEVEL_DEFAULT , RPC_C_IMP_LEVEL_IMPERSONATE , NULL , EOAC_NONE , NULL );

		if ( FAILED( hres ) )
		{
			CoUninitialize();
			return ReturnData;
		}

		IWbemLocator *pLoc = NULL;

		hres = CoCreateInstance( CLSID_WbemLocator , 0 , CLSCTX_INPROC_SERVER , IID_IWbemLocator , (LPVOID *)&pLoc );

		if ( FAILED( hres ) )
		{
			CoUninitialize();
			return ReturnData;
		}

		IWbemServices *pSvc = NULL;

		hres = pLoc->ConnectServer( _com_util::ConvertStringToBSTR( XorStr( "ROOT\\CIMV2" ) ) , NULL , NULL , 0 , NULL , 0 , 0 , &pSvc );

		if ( FAILED( hres ) )
		{
			pLoc->Release();
			CoUninitialize();
			return ReturnData;
		}

		hres = CoSetProxyBlanket( pSvc , RPC_C_AUTHN_WINNT , RPC_C_AUTHZ_NONE , NULL , RPC_C_AUTHN_LEVEL_CALL , RPC_C_IMP_LEVEL_IMPERSONATE , NULL , EOAC_NONE );

		if ( FAILED( hres ) )
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return ReturnData;
		}

		IEnumWbemClassObject* pEnumerator = NULL;

		hres = pSvc->ExecQuery(
			_com_util::ConvertStringToBSTR( XorStr( "WQL" ) ) ,
			_com_util::ConvertStringToBSTR( szQuery ) ,
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY ,
			NULL ,
			&pEnumerator );

		if ( FAILED( hres ) )
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return ReturnData;
		}

		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;

		while ( pEnumerator )
		{
			hres = pEnumerator->Next( WBEM_INFINITE , 1 , &pclsObj , &uReturn );

			if ( 0 == uReturn )
				break;

			VARIANT vtProp;
			VARIANT vtType;

			memset( &vtProp , 0 , sizeof( VARIANT ) );

			string InterfaceType = "";

			hres = pclsObj->Get( _com_util::ConvertStringToBSTR( szPropertie ) , 0 , &vtProp , 0 , 0 );

			if ( SUCCEEDED( hres ) && usb_drive )
			{
				pclsObj->Get( _com_util::ConvertStringToBSTR( XorStr( "InterfaceType" ) ) , 0 , &vtType , 0 , 0 );
				
				InterfaceType = _com_util::ConvertBSTRToString( vtType.bstrVal );

				if ( !InterfaceType.empty() && InterfaceType == XorStr( "USB" ) )
					continue;
			}

			string Value = "";

			if ( vtProp.bstrVal )
			{
				Value = _com_util::ConvertBSTRToString( vtProp.bstrVal );
				Value.erase( std::remove( Value.begin() , Value.end() , ' ' ) , Value.end() );
			}

			if ( !Value.empty() && Value.size() > 0 )
				ReturnData.push_back( Value );

			VariantClear( &vtProp );

			pclsObj->Release();
		}

		pSvc->Release();
		pLoc->Release();

		CoUninitialize();

		return ReturnData;
	}

	string GetHashText( const void* data , const size_t data_size )
	{
		HCRYPTPROV hProv = NULL;

		if ( !CryptAcquireContextA( &hProv , NULL , NULL , PROV_RSA_AES , CRYPT_VERIFYCONTEXT ) )
			return "";

		BOOL hash_ok = FALSE;
		HCRYPTPROV hHash = NULL;

		hash_ok = CryptCreateHash( hProv , CALG_MD5 , 0 , 0 , &hHash );

		if ( !hash_ok )
		{
			CryptReleaseContext( hProv , 0 );
			return "";
		}

		if ( !CryptHashData( hHash , static_cast<const BYTE *>( data ) , data_size , 0 ) )
		{
			CryptDestroyHash( hHash );
			CryptReleaseContext( hProv , 0 );
			return "";
		}

		DWORD cbHashSize = 0 , dwCount = sizeof( DWORD );

		if ( !CryptGetHashParam( hHash , HP_HASHSIZE , (BYTE *)&cbHashSize , &dwCount , 0 ) )
		{
			CryptDestroyHash( hHash );
			CryptReleaseContext( hProv , 0 );
			return "";
		}

		vector<BYTE> buffer( cbHashSize );

		if ( !CryptGetHashParam( hHash , HP_HASHVAL , reinterpret_cast<BYTE*>( &buffer[0] ) , &cbHashSize , 0 ) )
		{
			CryptDestroyHash( hHash );
			CryptReleaseContext( hProv , 0 );
			return "";
		}

		ostringstream oss;

		for ( vector<BYTE>::const_iterator iter = buffer.begin(); iter != buffer.end(); ++iter )
		{
			oss.fill( '0' );
			oss.width( 2 );
			oss << std::hex << static_cast<const int>( *iter );
		}

		CryptDestroyHash( hHash );
		CryptReleaseContext( hProv , 0 );

		return oss.str();
	}

	string StringToHex( string input )
	{
		const char* lut = XorStr( "0123456789ABCDEF" );
		size_t len = input.length();
		string output = "";

		output.reserve( 2 * len );

		for ( size_t i = 0; i < len; i++ )
		{
			const unsigned char c1 = input[i] >> 4;
			const unsigned char c2 = input[i] << 4;

			output.push_back( lut[c1] );
			output.push_back( lut[c2 >> 4] );
		}

		return output;
	}

	string GetHardwareID()
	{
		string HardwareID = "";
		static string HardwareUniqueKey = "";

		if ( !HardwareUniqueKey.empty() )
			return HardwareUniqueKey;

		vector<string> PhysicalMemory = GetWmiInfo( XorStr( "SELECT * FROM Win32_PhysicalMemory" ) , XorStr( "SerialNumber" ) );
		vector<string> DiskDriver = GetWmiInfo( XorStr( "SELECT * FROM Win32_DiskDrive" ) , XorStr( "SerialNumber" ) , true );
		vector<string> ComputerSystemProduct = GetWmiInfo( XorStr( "SELECT * FROM Win32_ComputerSystemProduct" ) , XorStr( "UUID" ) );

#if DEBUG_HARDWARE == 1
		printf( "PhysicalMemory: %i\n" , PhysicalMemory.size() );
		printf( "DiskDriver: %i\n" , DiskDriver.size() );
		printf( "ComputerSystemProduct: %i\n" , ComputerSystemProduct.size() );
#endif

		if ( DiskDriver.size() && ComputerSystemProduct.size() )
		{
			HardwareID += XorStr( "3D49C753B6217" );

			for ( auto Ram : PhysicalMemory )
				HardwareID += "-" + Ram;

			for ( auto Drive : DiskDriver )
				HardwareID += "-" + Drive;

			HardwareID += "-";
			HardwareID += ComputerSystemProduct[0];
			HardwareID += "-";
			HardwareID += XorStr( "91EC38FCD2235" );

			string HardwareIDHex = StringToHex( HardwareID );
			string HashHardwareID = GetHashText( HardwareIDHex.c_str() , HardwareIDHex.size() );

			for ( auto& c : HashHardwareID )
			{
				if ( c >= 'a' && c <= 'f' )
					c = toupper( c );
			}

			string Serial1 = HashHardwareID.substr( 4 , 4 );
			string Serial2 = HashHardwareID.substr( 8 , 4 );
			string Serial3 = HashHardwareID.substr( 24 , 4 );
			string Serial4 = HashHardwareID.substr( 28 , 4 );

			HardwareUniqueKey += Serial1;
			HardwareUniqueKey += '-';
			HardwareUniqueKey += Serial2;
			HardwareUniqueKey += '-';
			HardwareUniqueKey += Serial3;
			HardwareUniqueKey += '-';
			HardwareUniqueKey += Serial4;

#if DEBUG_HARDWARE == 1
			printf( "GetHardwareID: %s\n" , HardwareID.c_str() );
			printf( "HashHardwareID: %s\n" , HashHardwareID.c_str() );
			printf( "HardwareUniqueKey: %s\n" , HardwareUniqueKey.c_str() );
#endif
		}

		return HardwareUniqueKey;
	}
}