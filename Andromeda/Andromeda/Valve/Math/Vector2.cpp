#include "Matrix.hpp"
#include "Vector2.hpp"

Vector2::Vector2()
	: m_x( 0.f )
	, m_y( 0.f )
{ }

Vector2::Vector2( float x, float y )
	: m_x( x )
	, m_y( y )
{ }

Vector2::Vector2( const Vector2& vector )
	: m_x( vector[ 0 ] )
	, m_y( vector[ 1 ] )
{ }

Vector2::Vector2( const float* vector )
	: m_x( vector[ 0 ] )
	, m_y( vector[ 1 ] )
{ }

auto Vector2::Init( float x /* = 0.f */, float y /* = 0.f */ ) -> void
{
	m_x = x;
	m_y = y;
}

auto Vector2::ToArray() -> float*
{
	return reinterpret_cast< float* >( this );
}

auto Vector2::ToArray() const -> const float*
{
	return reinterpret_cast< const float* >( this );
}

auto Vector2::At( unsigned int index ) -> float&
{
	auto data = ToArray();
	return data[ index ];
}

auto Vector2::At( unsigned int index ) const -> const float
{
	auto data = ToArray();
	return data[ index ];
}

auto Vector2::IsEqual( const Vector2& vector ) const -> bool
{
	return ( m_x == vector[ 0 ] &&
			 m_y == vector[ 1 ] );
}

auto Vector2::Negate() -> void
{
	m_x = -m_x;
	m_y = -m_y;
}

auto Vector2::IsZero() -> bool
{
	return IsEqual( Vector2::Zero );
}

auto Vector2::Dot( const Vector2& vector ) const -> float
{
	return ( m_x * vector[ 0 ] + m_y * vector[ 1 ] );
}

auto Vector2::LengthSquared() const -> float
{
	return Dot( *this );
}

auto Vector2::Length() const -> float
{
	return std::sqrt( LengthSquared() );
}

auto Vector2::Add( const Vector2& vector ) -> void
{
	m_x += vector[ 0 ];
	m_y += vector[ 1 ];
}

auto Vector2::Subtract( const Vector2& vector ) -> void
{
	m_x -= vector[ 0 ];
	m_y -= vector[ 1 ];
}

auto Vector2::Multiply( const Vector2& vector ) -> void
{
	m_x *= vector[ 0 ];
	m_y *= vector[ 1 ];
}

auto Vector2::Divide( const Vector2& vector ) -> void
{
	m_x /= vector[ 0 ];
	m_y /= vector[ 1 ];
}

auto Vector2::Add( float value ) -> void
{
	m_x += value;
	m_y += value;
}

auto Vector2::Subtract( float value ) -> void
{
	m_x -= value;
	m_y -= value;
}

auto Vector2::Multiply( float value ) -> void
{
	m_x *= value;
	m_y *= value;
}

auto Vector2::Divide( float value ) -> void
{
	m_x /= value;
	m_y /= value;
}

auto Vector2::DistanceSquared( const Vector2& vector ) const -> float
{
	auto data = *this;
	data.Subtract( vector );
	return data.LengthSquared();
}

auto Vector2::Distance( const Vector2& vector ) const -> float
{
	auto data = *this;
	data.Subtract( vector );
	return data.Length();
}

auto Vector2::Normalize() -> float
{
	auto length = Length();
	auto length_normal = 1.f / length;

	m_x *= length_normal;
	m_y *= length_normal;

	return length;
}

auto Vector2::NormalizeFast() -> void
{
	auto length = Length();
	auto length_normal = 1.f / length;

	m_x *= length_normal;
	m_y *= length_normal;
}


auto Vector2::operator = ( const Vector2& vector ) -> Vector2&
{
	Init( vector[ 0 ], vector[ 1 ] );
	return *this;
}

auto Vector2::operator [] ( unsigned int index ) -> float&
{
	return At( index );
}

auto Vector2::operator [] ( unsigned int index ) const -> const float
{
	return At( index );
}

auto Vector2::operator == ( const Vector2& vector ) const -> bool
{
	return IsEqual( vector );
}

auto Vector2::operator != ( const Vector2& vector ) const -> bool
{
	return !IsEqual( vector );
}

auto Vector2::operator += ( const Vector2& vector ) -> Vector2&
{
	Add( vector );
	return *this;
}

auto Vector2::operator -= ( const Vector2& vector ) -> Vector2&
{
	Subtract( vector );
	return *this;
}

auto Vector2::operator *= ( const Vector2& vector ) -> Vector2&
{
	Multiply( vector );
	return *this;
}

auto Vector2::operator /= ( const Vector2& vector ) -> Vector2&
{
	Divide( vector );
	return *this;
}

auto Vector2::operator += ( float value ) -> Vector2&
{
	Add( value );
	return *this;
}

auto Vector2::operator -= ( float value ) -> Vector2&
{
	Subtract( value );
	return *this;
}

auto Vector2::operator *= ( float value ) -> Vector2&
{
	Multiply( value );
	return *this;
}

auto Vector2::operator /= ( float value ) -> Vector2&
{
	Divide( value );
	return *this;
}

auto Vector2::operator + ( const Vector2& vector ) const -> Vector2
{
	auto data = *this;
	data.Add( vector );
	return data;
}

auto Vector2::operator - ( const Vector2& vector ) const -> Vector2
{
	auto data = *this;
	data.Subtract( vector );
	return data;
}

auto Vector2::operator * ( const Vector2& vector ) const -> Vector2
{
	auto data = *this;
	data.Multiply( vector );
	return data;
}

auto Vector2::operator / ( const Vector2& vector ) const -> Vector2
{
	auto data = *this;
	data.Divide( vector );
	return data;
}

auto Vector2::operator + ( float value ) const -> Vector2
{
	auto data = *this;
	data.Add( value );
	return data;
}

auto Vector2::operator - ( float value ) const -> Vector2
{
	auto data = *this;
	data.Subtract( value );
	return data;
}

auto Vector2::operator * ( float value ) const -> Vector2
{
	auto data = *this;
	data.Multiply( value );
	return data;
}

auto Vector2::operator / ( float value ) const -> Vector2
{
	auto data = *this;
	data.Divide( value );
	return data;
}


Vector2 Vector2::Zero = { 0.f, 0.f };