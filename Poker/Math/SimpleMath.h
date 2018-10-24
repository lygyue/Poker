/********************************************************************
	created:	2016/06/27
	created:	27:6:2016   11:59
	file base:	SimpleMath
	file ext:	h
	author:		LYG
	
	purpose:	
*********************************************************************/
#ifndef __SIMPLE_MATH_H__
#define __SIMPLE_MATH_H__
#include <math.h>
#include <algorithm>
#include <vector>

#define PI			3.141592654f
#define TWO_PI		(2 * PI)
#define HALF_PI		(PI / 2)

//-----------------------------------------
//color defines
//-----------------------------------------
class Vector2;
class Vector3;
class Vector4;
#define Color_White              0xFFFFFFFF  //255,255,255
#define Color_LightGray          0xFFE0E0E0  //224,224,224
#define Color_Gray               0xFFC0C0C0  //192,192,192
#define Color_DarkGray           0xFF808080  //128,128,128
#define Color_Black              0           //0,0,0      

#define Color_Cyan               0xFF00FFFF  //0,255,255  
#define Color_DarkCyan           0xFF008080  //0,128,128  
#define Color_Purple             0xFFFF00FF  //255,0,255  
#define Color_DarkPurple         0xFF800080  //128,0,128  
#define Color_Yellow             0xFFFFFF00  //255,255,0  
#define Color_DarkYellow         0xFF808000  //128,128,0  

#define Color_Red                0xFFFF0000  //255,0,0    
#define Color_DarkRed            0xFF800000  //128,0,0    
#define Color_Green              0xFF00FF00  //0,255,0    
#define Color_DarkGreen          0xFF008000  //0,128,0    
#define Color_Blue               0xFF0000FF  //0,0,255    
#define Color_DarkBlue           0xFF000080  //0,0,128    

#define Color_Orange             0xFFFF8000

#define RGB_A(rgba)         (((DWORD)(rgba) & 0xFF000000) >> 24)
#define RGB_R(rgb)          (((DWORD)(rgb) & 0xFF0000) >> 16)
#define RGB_G(rgb)          (((DWORD)(rgb) & 0x00FF00) >> 8)
#define RGB_B(rgb)          ((DWORD)(rgb) & 0x0000FF)

#define RGB_Af(rgba)        (((float)RGB_A(rgba)) / 255.0f)
#define RGB_Rf(rgb)         (((float)RGB_R(rgb)) / 255.0f)
#define RGB_Gf(rgb)         (((float)RGB_G(rgb)) / 255.0f)
#define RGB_Bf(rgb)         (((float)RGB_B(rgb)) / 255.0f)

#define MAKERGBA(r,g,b,a)   ((((DWORD)a) << 24)|(((DWORD)r) << 16)|(((DWORD)g) << 8)|((DWORD)b))
#define MAKEBGRA(r,g,b,a)   ((((DWORD)a) << 24)|(((DWORD)b) << 16)|(((DWORD)g) << 8)|((DWORD)r))
#define MAKERGB(r,g,b)      ((((DWORD)r << 16)|((DWORD)g << 8)|(DWORD)b))
#define MAKEBGR(r,g,b)      ((((DWORD)b << 16)|((DWORD)g << 8)|(DWORD)r))

#define REVERSE_COLOR(col)  MAKERGB(RGB_B(col), RGB_G(col), RGB_R(col))

#define Vect4_to_RGBA(v)    (MAKERGBA(((v).x*255.0f), ((v).y*255.0f), ((v).z*255.0f), ((v).w*255.0f)))
#define Vect_to_RGB(v)      (MAKERGB(((v).x*255.0f), ((v).y*255.0f), ((v).z*255.0f)))
#define RGBA_to_Vect4(dw)   Vector4(RGB_Rf(dw), RGB_Gf(dw), RGB_Bf(dw), RGB_Af(dw))
#define RGB_to_Vect(dw)     Vector4(RGB_Rf(dw), RGB_Gf(dw), RGB_Bf(dw), 0)
#define RGB_to_VectExp(dw)  Vector4((RGB_Rf(dw)-0.5f)*2.0f, (RGB_Gf(dw)-0.5f)*2.0f, (RGB_Bf(dw)-0.5f)*2.0f, 0)

#define Color4_to_RGBA(c)   (MAKERGBA(((c).x*255.0f), ((c).y*255.0f), ((c).z*255.0f), ((c).w*255.0f)))
#define Color_to_RGB(c)     (MAKERGB(((c).x*255.0f), ((c).y*255.0f), ((c).z*255.0f)))
#define RGBA_to_Color4(dw)  Vector4(RGB_Rf(dw), RGB_Gf(dw), RGB_Bf(dw), RGB_Af(dw))
//#define RGB_to_Color(dw)    Color(RGB_Rf(dw), RGB_Gf(dw), RGB_Bf(dw))
//#define RGB_to_ColorExp(dw) Color((RGB_Rf(dw)-0.5f)*2.0f, (RGB_Gf(dw)-0.5f)*2.0f, (RGB_Bf(dw)-0.5f)*2.0f)

template <typename T>
inline static T Max(T left, T right)
{
	return left > right ? left : right;
}
template <typename T>
inline static T Min(T left, T right)
{
	return left > right ? right : left;
}

template <typename T>
inline static T Clamp(T val, T minval, T maxval)
{
	return Max(Min(val, maxval), minval);
}

extern bool RealEqual(float a, float b,
	float tolerance = std::numeric_limits<float>::epsilon());

extern inline float DegreesToRadians(float degrees);
extern inline float RadiansToDegrees(float radians);
extern inline bool isNaN(float f);
extern float InvSqrt(float fValue);
extern inline float Sqr(float fValue);
extern float UnitRandom();
extern float RangeRandom(float Min, float Max);

class Degree;

class Radian
{
	float mRad;

public:
	explicit Radian(float r = 0) : mRad(r) {}
	Radian(const Degree& d);
	Radian& operator = (const float& f) { mRad = f; return *this; }
	Radian& operator = (const Radian& r) { mRad = r.mRad; return *this; }
	Radian& operator = (const Degree& d);

	float valueDegrees() const; // see bottom of this file
	float valueRadians() const { return mRad; }

	const Radian& operator + () const { return *this; }
	Radian operator + (const Radian& r) const { return Radian(mRad + r.mRad); }
	Radian operator + (const Degree& d) const;
	Radian& operator += (const Radian& r) { mRad += r.mRad; return *this; }
	Radian& operator += (const Degree& d);
	Radian operator - () const { return Radian(-mRad); }
	Radian operator - (const Radian& r) const { return Radian(mRad - r.mRad); }
	Radian operator - (const Degree& d) const;
	Radian& operator -= (const Radian& r) { mRad -= r.mRad; return *this; }
	Radian& operator -= (const Degree& d);
	Radian operator * (float f) const { return Radian(mRad * f); }
	Radian operator * (const Radian& f) const { return Radian(mRad * f.mRad); }
	Radian& operator *= (float f) { mRad *= f; return *this; }
	Radian operator / (float f) const { return Radian(mRad / f); }
	Radian& operator /= (float f) { mRad /= f; return *this; }

	bool operator <  (const Radian& r) const { return mRad <  r.mRad; }
	bool operator <= (const Radian& r) const { return mRad <= r.mRad; }
	bool operator == (const Radian& r) const { return mRad == r.mRad; }
	bool operator != (const Radian& r) const { return mRad != r.mRad; }
	bool operator >= (const Radian& r) const { return mRad >= r.mRad; }
	bool operator >  (const Radian& r) const { return mRad >  r.mRad; }
};

/** Wrapper class which indicates a given angle value is in Degrees.
@remarks
Degree values are interchangeable with Radian values, and conversions
will be done automatically between them.
*/
class Degree
{
	float mDeg; // if you get an error here - make sure to define/typedef 'float' first

public:
	explicit Degree(float d = 0) : mDeg(d) {}
	Degree(const Radian& r) : mDeg(r.valueDegrees()) {}
	Degree& operator = (const float& f) { mDeg = f; return *this; }
	Degree& operator = (const Degree& d) { mDeg = d.mDeg; return *this; }
	Degree& operator = (const Radian& r) { mDeg = r.valueDegrees(); return *this; }

	float valueDegrees() const { return mDeg; }
	float valueRadians() const; // see bottom of this file

	const Degree& operator + () const { return *this; }
	Degree operator + (const Degree& d) const { return Degree(mDeg + d.mDeg); }
	Degree operator + (const Radian& r) const { return Degree(mDeg + r.valueDegrees()); }
	Degree& operator += (const Degree& d) { mDeg += d.mDeg; return *this; }
	Degree& operator += (const Radian& r) { mDeg += r.valueDegrees(); return *this; }
	Degree operator - () const { return Degree(-mDeg); }
	Degree operator - (const Degree& d) const { return Degree(mDeg - d.mDeg); }
	Degree operator - (const Radian& r) const { return Degree(mDeg - r.valueDegrees()); }
	Degree& operator -= (const Degree& d) { mDeg -= d.mDeg; return *this; }
	Degree& operator -= (const Radian& r) { mDeg -= r.valueDegrees(); return *this; }
	Degree operator * (float f) const { return Degree(mDeg * f); }
	Degree operator * (const Degree& f) const { return Degree(mDeg * f.mDeg); }
	Degree& operator *= (float f) { mDeg *= f; return *this; }
	Degree operator / (float f) const { return Degree(mDeg / f); }
	Degree& operator /= (float f) { mDeg /= f; return *this; }

	bool operator <  (const Degree& d) const { return mDeg <  d.mDeg; }
	bool operator <= (const Degree& d) const { return mDeg <= d.mDeg; }
	bool operator == (const Degree& d) const { return mDeg == d.mDeg; }
	bool operator != (const Degree& d) const { return mDeg != d.mDeg; }
	bool operator >= (const Degree& d) const { return mDeg >= d.mDeg; }
	bool operator >  (const Degree& d) const { return mDeg >  d.mDeg; }
};

// these functions could not be defined within the class definition of class
// Radian because they required class Degree to be defined
inline Radian::Radian(const Degree& d) : mRad(d.valueRadians()) {
}
inline Radian& Radian::operator = (const Degree& d) {
	mRad = d.valueRadians(); return *this;
}
inline Radian Radian::operator + (const Degree& d) const {
	return Radian(mRad + d.valueRadians());
}
inline Radian& Radian::operator += (const Degree& d) {
	mRad += d.valueRadians();
	return *this;
}
inline Radian Radian::operator - (const Degree& d) const {
	return Radian(mRad - d.valueRadians());
}
inline Radian& Radian::operator -= (const Degree& d) {
	mRad -= d.valueRadians();
	return *this;
}

inline float Radian::valueDegrees() const
{
	return RadiansToDegrees(mRad);
}


inline float Degree::valueRadians() const
{
	return DegreesToRadians(mDeg);
}

inline Radian operator * (float a, const Radian& b)
{
	return Radian(a * b.valueRadians());
}

inline Radian operator / (float a, const Radian& b)
{
	return Radian(a / b.valueRadians());
}

inline Degree operator * (float a, const Degree& b)
{
	return Degree(a * b.valueDegrees());
}

inline Degree operator / (float a, const Degree& b)
{
	return Degree(a / b.valueDegrees());
}

#endif
