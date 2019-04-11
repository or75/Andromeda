#include "License.h"

namespace License
{
	using namespace CryptoPP;

	// private key - decrypt server request
	/*
	MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBANRn97kHfHayAwrg9Ppv14At
	zhA31iRsXvyrCg3TIld06C/NMiC6yhA6ToxbLizAfgUSZ7VbLJTycQKHvuqzQyqQdS8PilCu
	cHQQ39U48wxGUcPRAVWcgeCUXb0pZJv3evAhTXjw1W3NLtLH3Ig2Ld/3jQXgeotYzNSfw6vM
	ZbPBAgERAoGADH6WGfFhrKEPPOAOaRWjQ8Z1iHvBTW/JWigtxJPy9hXvilddTTgqAPRe+TKK
	Pt4lh9Pn+5vzkEp/HkQ4aCipmPwe45RPH+GhF1cG1TlXS+vCGqtL9uYWFOoruusLBs0wGKn8
	txHvDYrL5jxtmmQwzjHQufQHDpIs+PSjNUKISTUCQQDr/sJktltO83PQ0JQjSqSogL7LJ7sN
	5EHdFGObJNY6SvLTS9CNrtLkHv0st6XaGDRLYEb0CydGFu3WHVqx37dPAkEA5mlP0ZLXY8pz
	eJsYRtxc9e0/pc41RyY7KmHv8oRSHP5aBDmAMka0sYWLl92htNf6XVBlt/dsXbleis/Hr3kf
	7wJBAIrSF/8B21ueRCB6sX4r6GMejlliqkRoJr5IOpd/FJrCrPTDL2Jm1mgSOpLGYY9ZiCxW
	wFNR2t3vXrovYoa/1T0CQEPEnwFnbIbDEuc8rMmMG1dj5YseiCP8L4TvoOz5vcxKzy5rQ9KN
	RDQ2Vju5qAgDSaL5h1Q5tnXrOeyXdvdf3DcCQQCaok/fXB+hhb3laODbQAvguceBZY4XbE0l
	MIxYtMTl0XEPK08mc1V2QzLx4Q5g5QHtEHW0zYtD+Q94VrUI5L4q
	*/

	// public key - encrypt client request 
	/*
	MIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQCeGS649a5jTKXcWwFG5IGlMwMDN7vINhZv
	kx2UrNWUcZzXhbRKC23kRogalec84zFexPHm4kE2Ns2eBxOeoJ6k9GLfFuHOJh4sw+1Lth/l
	BBnqmVUaxkYr/eSfIxDxPsbKMQLBitfVFxYV593de9SPMdQiaDiZs7KYIRiFJYad3QIBEQ==
	*/

	int RandomIntRange( int min , int max )
	{
		static bool first = true;

		if ( first )
		{
			srand( GetTickCount() );
			first = false;
		}

		return min + ( rand() % (int)( max - min + 1 ) );
	}

	string RequestBuffer = "";

	static size_t WriteCallback( void* contents , size_t size , size_t nmemb , void* userp )
	{
		size_t realsize = size * nmemb;
		RequestBuffer.append( (char*)contents , realsize );
		return realsize;
	}

	string GetUrlData( string& url )
	{
		string request_data = "";

		CURL* curl;
		CURLcode res;

		curl = curl_easy_init();

		if ( curl )
		{
			RequestBuffer.clear();

			string FullUrl = CHEAT_API;
			FullUrl += url;

			curl_easy_setopt( curl , CURLOPT_URL , FullUrl.c_str() );
			curl_easy_setopt( curl , CURLOPT_FOLLOWLOCATION , 1L );
			curl_easy_setopt( curl , CURLOPT_TIMEOUT , 5L );
			curl_easy_setopt( curl , CURLOPT_SSL_VERIFYPEER , 1L );
			curl_easy_setopt( curl , CURLOPT_SSL_VERIFYHOST , 2L );
			curl_easy_setopt( curl , CURLOPT_COOKIE , XorStr( "beget=begetok;" ) );

			curl_easy_setopt( curl , CURLOPT_WRITEFUNCTION , WriteCallback );

			res = curl_easy_perform( curl );

			if ( res != CURLE_OK )
				printf( "error: %s\n" , curl_easy_strerror( res ) );

			curl_easy_cleanup( curl );

			FullUrl.clear();

			return RequestBuffer;
		}

		return request_data;
	}

	string ToBase64( string buffer )
	{
		string base64_buffer = "";
		Base64Encoder encoder( new StringSink( base64_buffer ) );

		encoder.Put( (const byte*)buffer.data() , buffer.size() );
		encoder.MessageEnd();

		return base64_buffer;
	}

	string FromBase64( string buffer )
	{
		string base64_buffer = "";
		Base64Decoder decoder( new StringSink( base64_buffer ) );

		decoder.Put( (const byte*)buffer.data() , buffer.size() );
		decoder.MessageEnd();

		return base64_buffer;
	}

	string EncryptRsa( RSA::PublicKey& Key , string plainText )
	{
		AutoSeededRandomPool prng;
		string encrypted = "";
		RSAES_OAEP_SHA_Encryptor e( Key );
		StringSource( plainText , true , new PK_EncryptorFilter( prng , e , new StringSink( encrypted ) ) );
		return ToBase64( encrypted );
	}

	string DecryptRsa( RSA::PrivateKey& Key , string plainText )
	{
		AutoSeededRandomPool prng;
		string decrypted = "";
		RSAES_OAEP_SHA_Decryptor e( Key );
		StringSource( FromBase64( plainText ) , true , new PK_DecryptorFilter( prng , e , new StringSink( decrypted ) ) );
		return decrypted;
	}
	
	bool CheckLicense()
	{
		string PrivateKeyString = "MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBANRn97kHfHayAwrg9Ppv14AtzhA31iRsXvyrCg3TIld06C/NMiC6yhA6ToxbLizAfgUSZ7VbLJTycQKHvuqzQyqQdS8PilCucHQQ39U48wxGUcPRAVWcgeCUXb0pZJv3evAhTXjw1W3NLtLH3Ig2Ld/3jQXgeotYzNSfw6vMZbPBAgERAoGADH6WGfFhrKEPPOAOaRWjQ8Z1iHvBTW/JWigtxJPy9hXvilddTTgqAPRe+TKKPt4lh9Pn+5vzkEp/HkQ4aCipmPwe45RPH+GhF1cG1TlXS+vCGqtL9uYWFOoruusLBs0wGKn8txHvDYrL5jxtmmQwzjHQufQHDpIs+PSjNUKISTUCQQDr/sJktltO83PQ0JQjSqSogL7LJ7sN5EHdFGObJNY6SvLTS9CNrtLkHv0st6XaGDRLYEb0CydGFu3WHVqx37dPAkEA5mlP0ZLXY8pzeJsYRtxc9e0/pc41RyY7KmHv8oRSHP5aBDmAMka0sYWLl92htNf6XVBlt/dsXbleis/Hr3kf7wJBAIrSF/8B21ueRCB6sX4r6GMejlliqkRoJr5IOpd/FJrCrPTDL2Jm1mgSOpLGYY9ZiCxWwFNR2t3vXrovYoa/1T0CQEPEnwFnbIbDEuc8rMmMG1dj5YseiCP8L4TvoOz5vcxKzy5rQ9KNRDQ2Vju5qAgDSaL5h1Q5tnXrOeyXdvdf3DcCQQCaok/fXB+hhb3laODbQAvguceBZY4XbE0lMIxYtMTl0XEPK08mc1V2QzLx4Q5g5QHtEHW0zYtD+Q94VrUI5L4q";
		string PublicKeyString = "MIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQCeGS649a5jTKXcWwFG5IGlMwMDN7vINhZvkx2UrNWUcZzXhbRKC23kRogalec84zFexPHm4kE2Ns2eBxOeoJ6k9GLfFuHOJh4sw+1Lth/lBBnqmVUaxkYr/eSfIxDxPsbKMQLBitfVFxYV593de9SPMdQiaDiZs7KYIRiFJYad3QIBEQ==";
	
		StringSource SS_PrivateKey( PrivateKeyString , true );
		StringSource SS_PublicKey( PublicKeyString , true );

		RSA::PrivateKey privateKey;
		RSA::PublicKey publicKey;

		ByteQueue private_key_queue;
		ByteQueue public_key_queue;

		Base64Decoder decoder_priv( new Redirector( private_key_queue ) );
		Base64Decoder decoder_publ( new Redirector( public_key_queue ) );

		decoder_priv.Put( (const byte*)PrivateKeyString.data() , PrivateKeyString.size() );
		decoder_priv.MessageEnd();

		decoder_publ.Put( (const byte*)PublicKeyString.data() , PublicKeyString.size() );
		decoder_publ.MessageEnd();

		try
		{
			privateKey.Load( private_key_queue );
			publicKey.Load( public_key_queue );
		}
		catch ( const Exception& ex )
		{
			printf( XorStr( "error rsa: %s\n" ) , ex.what() );
			return false;
		}

		PrivateKeyString.clear();
		PublicKeyString.clear();

		string Hwid = HardwareID::GetHardwareID();
		string Ssid = to_string( RandomIntRange( 1000 , 10000 ) );

		string UserData = "";

		UserData += PRODUCT_KEY;
		UserData += XorStr( "|" ) + Hwid + XorStr( "|" ) + Ssid + XorStr( "|" ) + CHEAT_VERSION;

		string EncryptClientDataBase64 = EncryptRsa( publicKey , UserData );

		string UrlDataRequest = HardwareID::StringToHex( EncryptClientDataBase64 );
		string ReciveDataBase64 = GetUrlData( UrlDataRequest );

#if DEBUG_REQUEST == 1
		printf( XorStr( "ReciveDataBase64: %s\n" ) , ReciveDataBase64.c_str() );
#endif

		if ( ReciveDataBase64.empty() || ReciveDataBase64 == XorStr( "{BAD_REQUEST}" ) )
		{
			printf( "%s\n" , XorStr( "Ошибка запроса на сервер." ) );
			return false;
		}

		if ( ReciveDataBase64 == XorStr( "{BAD_SERVER_RSA}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			printf( "%s\n" , XorStr( "Ошибка на стороне сервера, обратитесь в группу." ) );
			return false;
		}

		string ReturnData = DecryptRsa( privateKey , ReciveDataBase64 );

#if DEBUG_REQUEST == 1
		printf( XorStr( "ReturnData: %s\n" ) , ReturnData.c_str() );
		getchar();

		return false;
#endif

		if ( ReturnData == XorStr( "{BAD_PRODUCT}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "На сервере отсутсвует данный продукт." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{BAD_HWID}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "На сервере отсутсвует пользователь с таким hwid." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{BAD_KEY}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "Ошибка ключа для индификации запроса." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{BAD_SUB}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "У вас отсутсвует подписка." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{PRODUCT_DISABLED}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "Продукт временно отключён. Ожидайте." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{SUB_PAUSED}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "Подписка приостановлена.Обратитесь к админу для возобновления подписки." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{SUB_BANED}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "Ваша подписка заблокирована." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{SUB_UPDATE}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "Доступно обновление для подписки! Обновите чит на сайте." ) );
			return false;
		}

		if ( ReturnData == XorStr( "{SUB_EXPIRED}" ) )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			printf( "%s\n" , XorStr( "Ваша подписка истекла !!! Продлите подписку используя ключ на сайте !!!" ) );
			return false;
		}

		string SuccessReturnData = "";

		SuccessReturnData.append( XorStr( "{SUB_ACTIVE}" ) );
		SuccessReturnData.append( XorStr( "-" ) );
		SuccessReturnData.append( Hwid );
		SuccessReturnData.append( XorStr( "-" ) );
		SuccessReturnData.append( Ssid );

		if ( SuccessReturnData == ReturnData )
		{
			Hwid = "";
			Ssid = "";
			UserData = "";
			EncryptClientDataBase64 = "";
			UrlDataRequest = "";
			ReciveDataBase64 = "";

			ReturnData = "";

			SuccessReturnData = "";

			printf( "%s\n" , XorStr( "[+] Подписка активна, запустите csgo." ) );
			return true;
		}

		return false;
	}
}