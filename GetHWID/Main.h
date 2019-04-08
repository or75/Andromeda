#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <vector>
#include <sstream>

#pragma comment(lib, "wbemuuid")

using namespace std;

#include "XorString/MetaString.h"

#define XorStr( str ) OBFUSCATED( str )

#include "HardwareID.h"