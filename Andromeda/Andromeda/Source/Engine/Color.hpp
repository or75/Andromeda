#pragma once

#include "../engine.hpp"

class Color
{
public:
	Color()
	{
		m_r = m_g = m_b = m_a = 0;
	}

	Color( BYTE r , BYTE g , BYTE b , BYTE a = 255 )
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}

	Color( D3DCOLOR code )
	{
		m_a = ( code >> 24 ) & 0xff;
		m_r = ( code >> 16 ) & 0xff;
		m_g = ( code >> 8 ) & 0xff;
		m_b = ( code ) & 0xff;
	}

	D3DCOLOR Code()
	{
		return D3DCOLOR_RGBA( m_r , m_g , m_b , m_a );
	}

	static Color None() { return Color( 0 , 0 , 0 , 0 ); }
	static Color White() { return Color( 255 , 255 , 255 ); }
	static Color Black() { return Color( 0 , 0 , 0 ); }
	static Color Red() { return Color( 255 , 0 , 0 ); }
	static Color Green() { return Color( 0 , 255 , 0 ); }
	static Color Blue() { return Color( 0 , 0 , 255 ); }
	static Color Cyan() { return Color( 0 , 255 , 255 ); }
	static Color Yellow() { return Color( 255 , 255 , 0 ); }

public:
	BYTE m_r , m_g , m_b , m_a;
};