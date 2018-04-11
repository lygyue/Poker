/********************************************************************
	created:	2016/06/27
	created:	27:6:2016   11:45
	file base:	Vector4
	file ext:	h
	author:		LYG
	
	purpose:	
*********************************************************************/
#ifndef __VECTOR4_H__
#define __VECTOR4_H__
#include <math.h>
#include <utility>
#include "Vector3.h"
class Vector4
{
public:
	float x, y, z, w;

public:
	/** Default constructor.
	@note
	It does <b>NOT</b> initialize the vector for efficiency.
	*/
	inline Vector4()
	{
	}

	inline Vector4(const float fX, const float fY, const float fZ, const float fW)
		: x(fX), y(fY), z(fZ), w(fW)
	{
	}

	inline explicit Vector4(const float afCoordinate[4])
		: x(afCoordinate[0]),
		y(afCoordinate[1]),
		z(afCoordinate[2]),
		w(afCoordinate[3])
	{
	}

	inline explicit Vector4(const int afCoordinate[4])
	{
		x = (float)afCoordinate[0];
		y = (float)afCoordinate[1];
		z = (float)afCoordinate[2];
		w = (float)afCoordinate[3];
	}

	inline explicit Vector4(float* const r)
		: x(r[0]), y(r[1]), z(r[2]), w(r[3])
	{
	}

	inline explicit Vector4(const float scaler)
		: x(scaler)
		, y(scaler)
		, z(scaler)
		, w(scaler)
	{
	}

	inline explicit Vector4(const Vector3& rhs)
		: x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
	{
	}

	/** Exchange the contents of this vector with another.
	*/
	inline void swap(Vector4& other)
	{
		std::swap(x, other.x);
		std::swap(y, other.y);
		std::swap(z, other.z);
		std::swap(w, other.w);
	}

	inline float operator [] (const size_t i) const
	{
		return *(&x + i);
	}

	inline float& operator [] (const size_t i)
	{
		return *(&x + i);
	}

	/// Pointer accessor for direct copying
	inline float* ptr()
	{
		return &x;
	}
	/// Pointer accessor for direct copying
	inline const float* ptr() const
	{
		return &x;
	}

	/** Assigns the value of the other vector.
	@param
	rkVector The other vector
	*/
	inline Vector4& operator = (const Vector4& rkVector)
	{
		x = rkVector.x;
		y = rkVector.y;
		z = rkVector.z;
		w = rkVector.w;

		return *this;
	}

	inline Vector4& operator = (const float fScalar)
	{
		x = fScalar;
		y = fScalar;
		z = fScalar;
		w = fScalar;
		return *this;
	}

	inline bool operator == (const Vector4& rkVector) const
	{
		return (x == rkVector.x &&
			y == rkVector.y &&
			z == rkVector.z &&
			w == rkVector.w);
	}

	inline bool operator != (const Vector4& rkVector) const
	{
		return (x != rkVector.x ||
			y != rkVector.y ||
			z != rkVector.z ||
			w != rkVector.w);
	}

	inline Vector4& operator = (const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0f;
		return *this;
	}

	// arithmetic operations
	inline Vector4 operator + (const Vector4& rkVector) const
	{
		return Vector4(
			x + rkVector.x,
			y + rkVector.y,
			z + rkVector.z,
			w + rkVector.w);
	}

	inline Vector4 operator - (const Vector4& rkVector) const
	{
		return Vector4(
			x - rkVector.x,
			y - rkVector.y,
			z - rkVector.z,
			w - rkVector.w);
	}

	inline Vector4 operator * (const float fScalar) const
	{
		return Vector4(
			x * fScalar,
			y * fScalar,
			z * fScalar,
			w * fScalar);
	}

	inline Vector4 operator * (const Vector4& rhs) const
	{
		return Vector4(
			rhs.x * x,
			rhs.y * y,
			rhs.z * z,
			rhs.w * w);
	}

	inline Vector4 operator / (const float fScalar) const
	{
		float fInv = 1.0f / fScalar;

		return Vector4(
			x * fInv,
			y * fInv,
			z * fInv,
			w * fInv);
	}

	inline Vector4 operator / (const Vector4& rhs) const
	{
		return Vector4(
			x / rhs.x,
			y / rhs.y,
			z / rhs.z,
			w / rhs.w);
	}

	inline const Vector4& operator + () const
	{
		return *this;
	}

	inline Vector4 operator - () const
	{
		return Vector4(-x, -y, -z, -w);
	}

	inline friend Vector4 operator * (const float fScalar, const Vector4& rkVector)
	{
		return Vector4(
			fScalar * rkVector.x,
			fScalar * rkVector.y,
			fScalar * rkVector.z,
			fScalar * rkVector.w);
	}

	inline friend Vector4 operator / (const float fScalar, const Vector4& rkVector)
	{
		return Vector4(
			fScalar / rkVector.x,
			fScalar / rkVector.y,
			fScalar / rkVector.z,
			fScalar / rkVector.w);
	}

	inline friend Vector4 operator + (const Vector4& lhs, const float rhs)
	{
		return Vector4(
			lhs.x + rhs,
			lhs.y + rhs,
			lhs.z + rhs,
			lhs.w + rhs);
	}

	inline friend Vector4 operator + (const float lhs, const Vector4& rhs)
	{
		return Vector4(
			lhs + rhs.x,
			lhs + rhs.y,
			lhs + rhs.z,
			lhs + rhs.w);
	}

	inline friend Vector4 operator - (const Vector4& lhs, float rhs)
	{
		return Vector4(
			lhs.x - rhs,
			lhs.y - rhs,
			lhs.z - rhs,
			lhs.w - rhs);
	}

	inline friend Vector4 operator - (const float lhs, const Vector4& rhs)
	{
		return Vector4(
			lhs - rhs.x,
			lhs - rhs.y,
			lhs - rhs.z,
			lhs - rhs.w);
	}

	// arithmetic updates
	inline Vector4& operator += (const Vector4& rkVector)
	{
		x += rkVector.x;
		y += rkVector.y;
		z += rkVector.z;
		w += rkVector.w;

		return *this;
	}

	inline Vector4& operator -= (const Vector4& rkVector)
	{
		x -= rkVector.x;
		y -= rkVector.y;
		z -= rkVector.z;
		w -= rkVector.w;

		return *this;
	}

	inline Vector4& operator *= (const float fScalar)
	{
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		w *= fScalar;
		return *this;
	}

	inline Vector4& operator += (const float fScalar)
	{
		x += fScalar;
		y += fScalar;
		z += fScalar;
		w += fScalar;
		return *this;
	}

	inline Vector4& operator -= (const float fScalar)
	{
		x -= fScalar;
		y -= fScalar;
		z -= fScalar;
		w -= fScalar;
		return *this;
	}

	inline Vector4& operator *= (const Vector4& rkVector)
	{
		x *= rkVector.x;
		y *= rkVector.y;
		z *= rkVector.z;
		w *= rkVector.w;

		return *this;
	}

	inline Vector4& operator /= (const float fScalar)
	{
		float fInv = 1.0f / fScalar;

		x *= fInv;
		y *= fInv;
		z *= fInv;
		w *= fInv;

		return *this;
	}

	inline Vector4& operator /= (const Vector4& rkVector)
	{
		x /= rkVector.x;
		y /= rkVector.y;
		z /= rkVector.z;
		w /= rkVector.w;

		return *this;
	}

	/** Calculates the dot (scalar) product of this vector with another.
	@param
	vec Vector with which to calculate the dot product (together
	with this one).
	@return
	A float representing the dot product value.
	*/
	inline float dotProduct(const Vector4& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}
	// special
	static const Vector4 ZERO;
};

#endif