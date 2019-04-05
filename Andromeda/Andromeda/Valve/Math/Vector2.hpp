#pragma once

#include "../../Include/Auto.hpp"

class Vector2
{
public:
	Vector2();
	Vector2( float x, float y );
	Vector2( const Vector2& vector );
	Vector2( const float* vector );

	auto Init( float x = 0.f, float y = 0.f ) -> void;

	auto ToArray() -> float*;
	auto ToArray() const -> const float*;

	auto At( unsigned int index ) -> float&;
	auto At( unsigned int index ) const -> const float;

	auto IsEqual( const Vector2& vector ) const -> bool;

	auto Add( const Vector2& vector ) -> void;
	auto Subtract( const Vector2& vector ) -> void;
	auto Multiply( const Vector2& vector ) -> void;
	auto Divide( const Vector2& vector ) -> void;

	auto Add( float value ) -> void;
	auto Subtract( float value ) -> void;
	auto Multiply( float value ) -> void;
	auto Divide( float value ) -> void;

	auto Negate() -> void;
	auto IsZero() -> bool;

	auto Dot( const Vector2& vector ) const -> float;

	auto LengthSquared() const -> float;
	auto Length() const -> float;

	auto DistanceSquared( const Vector2& vector ) const -> float;
	auto Distance( const Vector2& vector ) const -> float;

	auto Normalize() -> float;
	auto NormalizeFast() -> void;

public:
	auto operator = ( const Vector2& vector )->Vector2&;

	auto operator [] ( unsigned int index ) -> float&;
	auto operator [] ( unsigned int index ) const -> const float;

	auto operator == ( const Vector2& vector ) const -> bool;
	auto operator != ( const Vector2& vector ) const -> bool;

public:
	auto operator += ( const Vector2& vector )->Vector2&;
	auto operator -= ( const Vector2& vector )->Vector2&;
	auto operator *= ( const Vector2& vector )->Vector2&;
	auto operator /= ( const Vector2& vector )->Vector2&;

	auto operator += ( float value )->Vector2&;
	auto operator -= ( float value )->Vector2&;
	auto operator *= ( float value )->Vector2&;
	auto operator /= ( float value )->Vector2&;

	auto operator + ( const Vector2& vector ) const->Vector2;
	auto operator - ( const Vector2& vector ) const->Vector2;
	auto operator * ( const Vector2& vector ) const->Vector2;
	auto operator / ( const Vector2& vector ) const->Vector2;

	auto operator + ( float value ) const->Vector2;
	auto operator - ( float value ) const->Vector2;
	auto operator * ( float value ) const->Vector2;
	auto operator / ( float value ) const->Vector2;

public:
	static Vector2 Zero;

public:
	float m_x = 0.f;
	float m_y = 0.f;
};

inline auto operator * ( float value, const Vector2& vector ) -> Vector2
{
	return ( value * vector );
}