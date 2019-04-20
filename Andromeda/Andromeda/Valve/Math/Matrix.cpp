#include "Matrix.hpp"
#include "QAngle.hpp"
#include "Vector3.hpp"

auto matrix3x4_t::At( unsigned int index ) -> float*
{
	return m_data[ index ];
}

auto matrix3x4_t::At( unsigned int index ) const -> const float*
{
	return m_data[ index ];
}

auto matrix3x4_t::operator [] ( unsigned int index ) -> float*
{
	return At( index );
}

auto matrix3x4_t::operator [] ( unsigned int index ) const -> const float*
{
	return At( index );
}


auto VMatrix::At( unsigned int index ) -> float*
{
	return m_data[ index ];
}

auto VMatrix::At( unsigned int index ) const -> const float*
{
	return m_data[ index ];
}

auto VMatrix::operator [] ( unsigned int index ) -> float*
{
	return At( index );
}

auto VMatrix::operator [] ( unsigned int index ) const -> const float*
{
	return At( index );
}

auto AngleNormalize( float angle ) -> float
{
	angle = std::fmod( angle, 360.f );

	if( angle > 180.f )
		angle -= 360.f;
	else if( angle < -180.f )
		angle += 360.f;

	return angle;
}

void AngleVectors( const QAngle& QAngle , Vector3& vForward )
{
	float sp , sy , cp , cy;

	sp = sin( ToRadians( QAngle[0] ) );
	cp = cos( ToRadians( QAngle[0] ) );

	sy = sin( ToRadians( QAngle[1] ) );
	cy = cos( ToRadians( QAngle[1] ) );

	vForward.m_x = cp * cy;
	vForward.m_y = cp * sy;
	vForward.m_z = -sp;
}

void AngleVectors( const QAngle& QAngle , Vector3& vForward , Vector3& vRight , Vector3& vUp )
{
	float sr , sp , sy , cr , cp , cy;

	sp = sin( ToRadians( QAngle[0] ) );
	cp = cos( ToRadians( QAngle[0] ) );

	sy = sin( ToRadians( QAngle[1] ) );
	cy = cos( ToRadians( QAngle[1] ) );

	sr = sin( ToRadians( QAngle[2] ) );
	cr = cos( ToRadians( QAngle[2] ) );

	vForward.m_x = ( cp * cy );
	vForward.m_y = ( cp * sy );
	vForward.m_z = ( -sp );

	vRight.m_x = ( -1 * sr * sp * cy + -1 * cr * -sy );
	vRight.m_y = ( -1 * sr * sp * sy + -1 * cr *  cy );
	vRight.m_z = ( -1 * sr * cp );

	vUp.m_x = ( cr * sp * cy + -sr * -sy );
	vUp.m_y = ( cr * sp * sy + -sr * cy );
	vUp.m_z = ( cr * cp );
}

void VectorAngles( const Vector3& vForward , QAngle& QAngle )
{
	float tmp , yaw , pitch;

	if ( vForward[1] == 0 && vForward[0] == 0 )
	{
		yaw = 0;

		if ( vForward[2] > 0 )
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = ( atan2( vForward[1] , vForward[0] ) * g_pi_degrees / g_pi_radians );

		if ( yaw < 0 )
			yaw += 360;

		tmp = sqrt( vForward[0] * vForward[0] + vForward[1] * vForward[1] );
		pitch = ( atan2( -vForward[2] , tmp ) * g_pi_degrees / g_pi_radians );

		if ( pitch < 0 )
			pitch += 360;
	}

	QAngle[0] = pitch;
	QAngle[1] = yaw;
	QAngle[2] = 0;
}
