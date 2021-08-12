#pragma once

#include "Main.h"

#define LICENSE_CHECK 1
#define DEBUG_REQUEST 0

#define CHEAT_API		XorStr( "http://v-hack.ru/api/license/" )

#define PRODUCT_KEY		XorStr( "AD57F742956BC" )
#define CHEAT_VERSION	XorStr( "8" )
#define DLL_LIC_KEY		0xBF46F

namespace License
{
	string GetUrlData( string& url );
	bool CheckLicense();
}