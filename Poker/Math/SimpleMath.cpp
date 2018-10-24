/********************************************************************
	created:	2016/06/27
	created:	27:6:2016   12:12
	file base:	SimpleMath
	file ext:	cpp
	author:		LYG
	
	purpose:	
*********************************************************************/
#include "SimpleMath.h"
#include <time.h>
#include "Matrix4.h"

//-----------------------------------------------------------------------
bool RealEqual(float a, float b, float tolerance)
{
	if (fabs(b - a) <= tolerance)
		return true;
	else
		return false;
}

float DegreesToRadians(float degrees)
{
	return degrees * PI / 180.0f;
}

float RadiansToDegrees(float radians)
{
	return radians * 180.0f / PI;
}

bool isNaN(float f)
{
	// std::isnan() is C99, not supported by all compilers
	// However NaN always fails this next test, no other number does.
	return f != f;
}

float InvSqrt(float fValue)
{
	return 1.0f / sqrt(fValue);
}

float Sqr(float fValue)
{
	return fValue*fValue;
}

float UnitRandom()
{
	static bool firstRun = true;
	if (firstRun)
	{
		srand((unsigned)time(NULL));
		firstRun = false;
	}
	return float(rand()) / float(RAND_MAX);
}

float RangeRandom(float Min, float Max)
{
	return (Max - Min)*UnitRandom() + Min;
}

