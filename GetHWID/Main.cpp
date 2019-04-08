#include "Main.h"

void CopyToClipBoard( const char* pStr )
{
	if ( OpenClipboard( 0 ) )
	{
		HGLOBAL hgBuffer;
		char* chBuffer;
		size_t len_str = strlen( pStr ) + 1;
		EmptyClipboard();
		hgBuffer = GlobalAlloc( GMEM_DDESHARE , len_str );
		chBuffer = (char*)GlobalLock( hgBuffer );
		strcpy_s( chBuffer , len_str , LPCSTR( pStr ) );
		GlobalUnlock( hgBuffer );
		SetClipboardData( CF_TEXT , hgBuffer );
		CloseClipboard();
	}
}

int main( int argc , char** argv )
{
	setlocale( LC_ALL , XorStr( "Russian" ) );

	string HardwareID = HardwareID::GetHardwareID();

	printf( XorStr( "Ваш HWID (используйте ctrl + v при регистрации для вставки): (%s)\n" ) , HardwareID.c_str() );

	CopyToClipBoard( HardwareID.c_str() );

	getchar();
	return 0;
}