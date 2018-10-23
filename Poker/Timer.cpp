/********************************************************************
	created:	2017/09/05
	created:	5:9:2017   9:32
	file base:	Timer
	file ext:	cpp
	author:		LYG
	
	purpose:	
*********************************************************************/
#include "Timer.h"
#include <algorithm>
Timer::Timer()
: mTimerMask(0)
, mDelta(0)
{
	reset();
}
//-------------------------------------------------------------------------
Timer::~Timer()
{

}
//-------------------------------------------------------------------------
Timer* Timer::ThisInstance = nullptr;
//-------------------------------------------------------------------------
Timer* Timer::GetInstance()
{
	return ThisInstance;
}
//-------------------------------------------------------------------------
bool Timer::Initialise()
{
	return true;
}
//-------------------------------------------------------------------------
bool Timer::Update()
{
	// 计算两帧之间时间间隔
	static unsigned long LastTime = getMilliseconds();
	unsigned long CurrentTime = getMilliseconds();
	mDelta = CurrentTime - LastTime;
	LastTime = CurrentTime;
	// 回调定时器
	for (unsigned int i = 0; i < mTimerEventList.size(); i++)
	{
		mTimerEventList[i].RemainTime += mDelta;
		if (mTimerEventList[i].RemainTime >= mTimerEventList[i].Delta)
		{
			// 函数回调
			mTimerEventList[i].Listener->OnTimer(mTimerEventList[i].EventID);
			// 需要重置时间。这个重置用求模重置，如果定时器设置过快，会丢弃一些回调。
			// 举例：Timer定时器时间设置为1，你大概率无法保证每秒回调1000次。
			mTimerEventList[i].RemainTime %= mTimerEventList[i].Delta;
		}
	}
	for (int i = mTimerEventOnceList.size() - 1; i >= 0; i--)
	{
		mTimerEventOnceList[i].RemainTime += mDelta;
		if (mTimerEventOnceList[i].RemainTime >= mTimerEventOnceList[i].Delta)
		{
			mTimerEventOnceList[i].Listener->OnTimer(mTimerEventOnceList[i].EventID);
			mTimerEventOnceList.erase(mTimerEventOnceList.begin() + i);
		}
	}
	return true;
}
//-------------------------------------------------------------------------
unsigned long Timer::GetDelta()
{
	return mDelta;
}

//-------------------------------------------------------------------------
void Timer::AddTimer(ITimerListener* Listener, unsigned int EventID, unsigned int Delta)
{
	// 不做任何错误检测，全靠外部调用注意。这东西做错误检查也没什么用，按道理外面调用不该出错。
	TimerObject obj;
	obj.Listener = Listener;
	obj.EventID = EventID;
	obj.Delta = Delta;
	obj.RemainTime = 0;
	mTimerEventList.push_back(obj);
}
//-------------------------------------------------------------------------
void Timer::RemoveTimer(ITimerListener* Listener, unsigned int EventID)
{
	std::vector<TimerObject>::iterator it;
	for (it = mTimerEventList.begin(); it != mTimerEventList.end(); it++)
	{
		if (it->Listener == Listener && it->EventID == EventID)
		{
			mTimerEventList.erase(it);
			break;
		}
	}
	return;
}
//-------------------------------------------------------------------------
void Timer::AddOnceTimer(ITimerListener* Listener, unsigned int EventID, unsigned int Delta)
{
	TimerObject obj;
	obj.Listener = Listener;
	obj.EventID = EventID;
	obj.Delta = Delta;
	obj.RemainTime = 0;
	mTimerEventOnceList.push_back(obj);
}
//-------------------------------------------------------------------------
template<typename T>
static FORCEINLINE bool isPO2(T n)
{
	return (n & (n - 1)) == 0;
}
//-------------------------------------------------------------------------
bool Timer::setOption(const std::string & key, const void * val)
{
	if (key == "QueryAffinityMask")
	{
		// Telling timer what core to use for a timer read
		DWORD newTimerMask = *static_cast <const DWORD *> (val);

		// Get the current process core mask
		DWORD_PTR procMask;
		DWORD_PTR sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// If new mask is 0, then set to default behavior, otherwise check
		// to make sure new timer core mask overlaps with process core mask
		// and that new timer core mask is a power of 2 (i.e. a single core)
		if ((newTimerMask == 0) ||
			(((newTimerMask & procMask) != 0) && isPO2(newTimerMask)))
		{
			mTimerMask = newTimerMask;
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------
void Timer::reset()
{
	// Get the current process core mask
	DWORD_PTR procMask;
	DWORD_PTR sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	// If procMask is 0, consider there is only one core available
	// (using 0 as procMask will cause an infinite loop below)
	if (procMask == 0)
		procMask = 1;

	// Find the lowest core that this process uses
	if (mTimerMask == 0)
	{
		mTimerMask = 1;
		while ((mTimerMask & procMask) == 0)
		{
			mTimerMask <<= 1;
		}
	}

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Get the constant frequency
	QueryPerformanceFrequency(&mFrequency);

	// Query the timer
	QueryPerformanceCounter(&mStartTime);
	mStartTick = GetTickCount();

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	mLastTime = 0;
	mZeroClock = clock();
}

//-------------------------------------------------------------------------
unsigned long Timer::getMilliseconds()
{
	LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

	// scale by 1000 for milliseconds
	unsigned long newTicks = (unsigned long)(1000 * newTime / mFrequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - mStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{
		// We must keep the timer running forward :)
		LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
		mStartTime.QuadPart += adjust;
		newTime -= adjust;

		// Re-calculate milliseconds
		newTicks = (unsigned long)(1000 * newTime / mFrequency.QuadPart);
	}

	// Record last time for adjust
	mLastTime = newTime;

	return newTicks;
}

//-------------------------------------------------------------------------
unsigned long Timer::getMicroseconds()
{
	LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);


	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);


	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

	// get milliseconds to check against GetTickCount
	unsigned long newTicks = (unsigned long)(1000 * newTime / mFrequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - mStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{
		// We must keep the timer running forward :)
		LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
		mStartTime.QuadPart += adjust;
		newTime -= adjust;
	}

	// Record last time for adjust
	mLastTime = newTime;

	// scale by 1000000 for microseconds
	unsigned long newMicro = (unsigned long)(1000000 * newTime / mFrequency.QuadPart);

	return newMicro;
}

//-------------------------------------------------------------------------
unsigned long Timer::getMillisecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)((float)(newClock - mZeroClock) / ((float)CLOCKS_PER_SEC / 1000.0));
}

//-------------------------------------------------------------------------
unsigned long Timer::getMicrosecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)((float)(newClock - mZeroClock) / ((float)CLOCKS_PER_SEC / 1000000.0));
}
