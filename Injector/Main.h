#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <sstream>

using namespace std;

#define CURL_STATICLIB

#include "curl/curl.h"

#include "cryptlib/rsa.h"
#include "cryptlib/osrng.h"
#include "cryptlib/base64.h"
#include "cryptlib/files.h"
#include "cryptlib/hex.h"

#pragma comment(lib,"curl")
#pragma comment(lib,"cryptlib")
#pragma comment(lib,"wbemuuid")
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"Wldap32")
#pragma comment(lib,"crypt32")

#include "XorString/MetaString.h"

#define XorStr( str ) OBFUSCATED( str )

#include "HardwareID.h"
#include "License.h"
#include "Inject.h"