//
//  MetaString.h
//  ADVobfuscator
//
// Copyright (c) 2010-2017, Sebastien Andrivet
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Get latest version on https://github.com/andrivet/ADVobfuscator

#ifndef MetaString_h
#define MetaString_h

/*
#include "Inline.h"
#include "Indexes.h"
#include "MetaRandom.h"
#include "Log.h"

namespace andrivet { namespace ADVobfuscator {

  // Represents an obfuscated string, parametrized with an alrorithm number N, a list of indexes Indexes and a key Key

  template<int N, char Key, typename Indexes>
  struct MetaString;

  // Partial specialization with a list of indexes I, a key K and algorithm N = 0
  // Each character is encrypted (XOR) with the same key

  template<char K, int... I>
  struct MetaString<0, K, Indexes<I...>>
  {
    // Constructor. Evaluated at compile time.
    constexpr ALWAYS_INLINE MetaString(const char* str)
    : key_{K}, buffer_ {encrypt(str[I], K)...} { }

    // Runtime decryption. Most of the time, inlined
    inline const char* decrypt()
    {
      for(size_t i = 0; i < sizeof...(I); ++i)
        buffer_[i] = decrypt(buffer_[i]);
      buffer_[sizeof...(I)] = 0;
      LOG("--- Implementation #" << 0 << " with key 0x" << hex(key_));
      return const_cast<const char*>(buffer_);
    }

  private:
    // Encrypt / decrypt a character of the original string with the key
    constexpr char key() const { return key_; }
    constexpr char ALWAYS_INLINE encrypt(char c, int k) const { return c ^ k; }
    constexpr char decrypt(char c) const { return encrypt(c, key()); }

    volatile int key_; // key. "volatile" is important to avoid uncontrolled over-optimization by the compiler
    volatile char buffer_[sizeof...(I) + 1]; // Buffer to store the encrypted string + terminating null byte
  };

  // Partial specialization with a list of indexes I, a key K and algorithm N = 1
  // Each character is encrypted (XOR) with an incremented key.

  template<char K, int... I>
  struct MetaString<1, K, Indexes<I...>>
  {
    // Constructor. Evaluated at compile time.
    constexpr ALWAYS_INLINE MetaString(const char* str)
    : key_(K), buffer_ {encrypt(str[I], I)...} { }

    // Runtime decryption. Most of the time, inlined
    inline const char* decrypt()
    {
      for(size_t i = 0; i < sizeof...(I); ++i)
        buffer_[i] = decrypt(buffer_[i], i);
      buffer_[sizeof...(I)] = 0;
      LOG("--- Implementation #" << 1 << " with key 0x" << hex(key_));
      return const_cast<const char*>(buffer_);
    }

  private:
    // Encrypt / decrypt a character of the original string with the key
    constexpr char key(size_t position) const { return static_cast<char>(key_ + position); }
    constexpr char ALWAYS_INLINE encrypt(char c, size_t position) const { return c ^ key(position); }
    constexpr char decrypt(char c, size_t position) const { return encrypt(c, position); }

    volatile int key_; // key. "volatile" is important to avoid uncontrolled over-optimization by the compiler
    volatile char buffer_[sizeof...(I) + 1]; // Buffer to store the encrypted string + terminating null byte
  };

  // Partial specialization with a list of indexes I, a key K and algorithm N = 2
  // Shift the value of each character and does not store the key. It is only used at compile-time.

  template<char K, int... I>
  struct MetaString<2, K, Indexes<I...>>
  {
    // Constructor. Evaluated at compile time. Key is *not* stored
    constexpr ALWAYS_INLINE MetaString(const char* str)
    : buffer_ {encrypt(str[I])..., 0} { }

    // Runtime decryption. Most of the time, inlined
    inline const char* decrypt()
    {
      for(size_t i = 0; i < sizeof...(I); ++i)
        buffer_[i] = decrypt(buffer_[i]);
      LOG("--- Implementation #" << 2 << " with key 0x" << hex(K));
      return const_cast<const char*>(buffer_);
    }

  private:
    // Encrypt / decrypt a character of the original string with the key
    // Be sure that the encryption key is never 0.
    constexpr char key(char key) const { return 1 + (key % 13); }
    constexpr char ALWAYS_INLINE encrypt(char c) const { return c + key(K); }
    constexpr char decrypt(char c) const { return c - key(K); }

    // Buffer to store the encrypted string + terminating null byte. Key is not stored
    volatile char buffer_[sizeof...(I) + 1];
  };

  // Helper to generate a key
  template<int N>
  struct MetaRandomChar
  {
    // Use 0x7F as maximum value since most of the time, char is signed (we have however 1 bit less of randomness)
    static const char value = static_cast<char>(1 + MetaRandom<N, 0x7F - 1>::value);
  };


}}

using namespace andrivet::ADVobfuscator;

// Prefix notation
#define DEF_OBFUSCATED(str) MetaString<andrivet::ADVobfuscator::MetaRandom<__COUNTER__, 3>::value, andrivet::ADVobfuscator::MetaRandomChar<__COUNTER__>::value, Make_Indexes<sizeof(str) - 1>::type>(str)
#define OBFUSCATED(str) ((char*)DEF_OBFUSCATED(str).decrypt())
#define XorStr( str ) OBFUSCATED( str )
*/

#include <string>
#include <array>
#include <cstdarg>

#define BEGIN_NAMESPACE( x ) namespace x {
#define END_NAMESPACE }

BEGIN_NAMESPACE( XorCompileTime )

constexpr auto time = __TIME__;
constexpr auto seed = static_cast<int>( time[7] ) + static_cast<int>( time[6] ) * 10 + static_cast<int>( time[4] ) * 60 + static_cast<int>( time[3] ) * 600 + static_cast<int>( time[1] ) * 3600 + static_cast<int>( time[0] ) * 36000;

template < int N >
struct RandomGenerator {
private:
	static constexpr unsigned a = 16807; // 7^5
	static constexpr unsigned m = 2147483647; // 2^31 - 1

	static constexpr unsigned s = RandomGenerator< N - 1 >::value;
	static constexpr unsigned lo = a * ( s & 0xFFFF ); // Multiply lower 16 bits by 16807
	static constexpr unsigned hi = a * ( s >> 16 ); // Multiply higher 16 bits by 16807
	static constexpr unsigned lo2 = lo + ( ( hi & 0x7FFF ) << 16 ); // Combine lower 15 bits of hi with lo's upper bits
	static constexpr unsigned hi2 = hi >> 15; // Discard lower 15 bits of hi
	static constexpr unsigned lo3 = lo2 + hi;

public:
	static constexpr unsigned max = m;
	static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
};

template <>
struct RandomGenerator< 0 > {
	static constexpr unsigned value = seed;
};

template < int N , int M >
struct RandomInt {
	static constexpr auto value = RandomGenerator< N + 1 >::value % M;
};

template < int N >
struct RandomChar {
	static const char value = static_cast<char>( 1 + RandomInt< N , 0x7F - 1 >::value );
};

template < size_t N , int K >
struct XorString {
private:
	const char _key;
	std::array< char , N + 1 > _encrypted;

	constexpr char enc( char c ) const
	{
		return c ^ _key;
	}

	char dec( char c ) const
	{
		return c ^ _key;
	}

public:
	template < size_t... Is >
	constexpr __forceinline XorString( const char* str , std::index_sequence< Is... > ) : _key( RandomChar< K >::value ) , _encrypted{ enc( str[Is] )... } {
	}

	__forceinline decltype( auto ) decrypt( void )
	{
		for ( size_t i = 0; i < N; ++i )
		{
			_encrypted[i] = dec( _encrypted[i] );
		}
		_encrypted[N] = '\0';
		return _encrypted.data();
	}
};

END_NAMESPACE

#define OBFUSCATED( s ) ( (char*)XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#define XorStr( str ) OBFUSCATED( str )

#endif
