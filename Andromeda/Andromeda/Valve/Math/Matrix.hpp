#pragma once

#include "../../Include/Auto.hpp"

constexpr auto g_pi_radians = static_cast< float >( 3.14159265358979323846 );
constexpr auto g_pi_degrees = static_cast< float >( 180.0 );

struct matrix3x4_t
{
public:
	auto At( unsigned int index ) -> float*;
	auto At( unsigned int index ) const -> const float*;

public:
	auto operator [] ( unsigned int index ) -> float*;
	auto operator [] ( unsigned int index ) const -> const float*;

private:
	float m_data[ 3 ][ 4 ] = { };
};

class VMatrix
{
public:
	auto At( unsigned int index ) -> float*;
	auto At( unsigned int index ) const -> const float*;

public:
	auto operator [] ( unsigned int index ) -> float*;
	auto operator [] ( unsigned int index ) const -> const float*;

private:
	float m_data[ 4 ][ 4 ] = { };
};

class QAngle;
class Vector3;
class Vector2;
class Vector4;

constexpr auto ToRadians( float degrees ) -> float
{
	return ( degrees * static_cast< float >( g_pi_radians / g_pi_degrees ) );
}

constexpr auto ToDegrees( float radians ) -> float
{
	return ( radians * static_cast< float >( g_pi_degrees / g_pi_radians ) );
}

auto AngleNormalize( float angle ) -> float;

void AngleVectors( const QAngle& QAngle , Vector3& vForward );
void AngleVectors( const QAngle& QAngle , Vector3& vForward , Vector3& vRight , Vector3& vUp );

void VectorAngles( const Vector3& vForward , QAngle& QAngle );