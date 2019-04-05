#pragma once

#include "../../Include/Auto.hpp"

struct matrix3x4_t;

class QAngle;
class Vector4;

class Vector3
{
public:
	Vector3();
	Vector3( float x, float y, float z );
	Vector3( const Vector3& vector );
	Vector3( const float* vector );

	auto Init( float x = 0.f, float y = 0.f, float z = 0.f ) -> void;

	auto ToArray() -> float*;
	auto ToArray() const -> const float*;

	auto At( unsigned int index ) -> float&;
	auto At( unsigned int index ) const -> const float;

	auto IsEqual( const Vector3& vector ) const -> bool;
	
	auto Add( const Vector3& vector ) -> void;
	auto Subtract( const Vector3& vector ) -> void;
	auto Multiply( const Vector3& vector ) -> void;
	auto Divide( const Vector3& vector ) -> void;

	auto Add( float value ) -> void;
	auto Subtract( float value ) -> void;
	auto Multiply( float value ) -> void;
	auto Divide( float value ) -> void;

	auto IsZero() const -> bool;

	auto Dot( const Vector3& vector ) const -> float;

	auto LengthSquared() const -> float;
	auto Length() const -> float;

	auto Length2DSquared() const -> float;
	auto Length2D() const -> float;

	auto DistanceSquared( const Vector3& vector ) const -> float;
	auto Distance( const Vector3& vector ) const -> float;

	auto Normalize() -> float;
	auto NormalizeFast() -> void;

	auto Transform( const matrix3x4_t& transform ) const -> Vector3;
	auto Cross( const Vector3& vector ) -> Vector3;

	auto ToVector4D( float w = 0.f ) const -> Vector4;

public:
	auto operator = ( const Vector3& vector ) -> Vector3&;

	auto operator [] ( unsigned int index ) -> float&;
	auto operator [] ( unsigned int index ) const -> const float;

	auto operator == ( const Vector3& vector ) const -> bool;
	auto operator != ( const Vector3& vector ) const -> bool;

public:
	auto operator += ( const Vector3& vector ) -> Vector3&;
	auto operator -= ( const Vector3& vector ) -> Vector3&;
	auto operator *= ( const Vector3& vector ) -> Vector3&;
	auto operator /= ( const Vector3& vector ) -> Vector3&;

	auto operator += ( float value ) -> Vector3&;
	auto operator -= ( float value ) -> Vector3&;
	auto operator *= ( float value ) -> Vector3&;
	auto operator /= ( float value ) -> Vector3&;

	auto operator + ( const Vector3& vector ) const -> Vector3;
	auto operator - ( const Vector3& vector ) const -> Vector3;
	auto operator * ( const Vector3& vector ) const -> Vector3;
	auto operator / ( const Vector3& vector ) const -> Vector3;

	auto operator + ( float value ) const -> Vector3;
	auto operator - ( float value ) const -> Vector3;
	auto operator * ( float value ) const -> Vector3;
	auto operator / ( float value ) const -> Vector3;

public:
	static Vector3 Zero;

public:
	float m_x = 0.f;
	float m_y = 0.f;
	float m_z = 0.f;
};

inline auto operator * ( float value, const Vector3& vector ) -> Vector3
{
	return ( value * vector );
}