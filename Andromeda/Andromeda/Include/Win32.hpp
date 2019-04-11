#pragma once

#include <Windows.h>
#include <ShlObj.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Psapi.h>

#pragma comment(lib,"d3d9")
#pragma comment(lib,"d3dx9")

#pragma comment(lib,"psapi")

#include "Config.hpp"

// XorStr
#if ENABLE_XOR_STR == 0
#define XorStr(str) str
#else
#include "XorString/MetaString.h"
#endif

template< class T >
class Singleton
{
public:
	Singleton() = default;

	static auto Instance() -> T&
	{
		static T instance;
		return instance;
	}

protected:
	Singleton( const Singleton& ) = delete;
	auto operator = ( const Singleton& )->Singleton& = delete;
};
