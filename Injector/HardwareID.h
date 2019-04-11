#pragma once

#include "Main.h"

#define DEBUG_HARDWARE 0

namespace HardwareID
{
	vector<string> GetWmiInfo( const char* szQuery , const char* szPropertie , bool usb_drive = false );
	string GetHashText( const void* data , const size_t data_size );
	string StringToHex( string input );
	string GetHardwareID();
}