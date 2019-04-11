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

bool InitalizeInjector()
{
	setlocale( LC_ALL , "Russian" );

#if LICENSE_CHECK == 1
	if ( !License::CheckLicense() )
	{
		Sleep( 5000 );
		return false;
	}
#endif

	Inject::InjectCSGO();

	return true;
}

int main( int argc , char** argv )
{
	return InitalizeInjector();
}