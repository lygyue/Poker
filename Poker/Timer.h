/********************************************************************
	created:	2017/09/05
	created:	5:9:2017   9:26
	file base:	Timer
	file ext:	h
	author:		LYG
	
	purpose:	
*********************************************************************/
#ifndef		__TIMER_H__
#define		__TIMER_H__
#include <windows.h>
#include <time.h>
#include <xstring>
#include <vector>

struct ITimerListener
{
	virtual void OnTimer(unsigned int EventID) = 0;
};

struct TimerObject
{
	ITimerListener* Listener;
	unsigned int EventID;
	unsigned int Delta;
	unsigned int RemainTime;
	TimerObject()
	{
		memset(this, 0, sizeof(TimerObject));
	}
};

class Timer
{
	friend class PokerLogicManager;
protected:
	Timer();
	~Timer();

	static Timer* ThisInstance;
public:
	// 初始化
	bool Initialise();
	static Timer* GetInstance();
	// 更新
	bool Update();
	unsigned long GetDelta();
	/** Returns milliseconds since initialisation or last reset */
	unsigned long getMilliseconds();
	/** Returns microseconds since initialisation or last reset */
	unsigned long getMicroseconds();
	void AddTimer(ITimerListener* Listener, unsigned int EventID, unsigned int Delta);
	void RemoveTimer(ITimerListener* Listener, unsigned int EventID);
	// Add a once callback timer, which been auto remove.
	void AddOnceTimer(ITimerListener* Listener, unsigned int EventID, unsigned int Delta);
private:
	bool setOption(const std::string& strKey, const void* pValue);
	/** Resets timer */
	void reset();
	/** Returns milliseconds since initialisation or last reset, only CPU time measured */
	unsigned long getMillisecondsCPU();
	/** Returns microseconds since initialisation or last reset, only CPU time measured */
	unsigned long getMicrosecondsCPU();

	DWORD GetTickCount() { return (DWORD)GetTickCount64(); }

	clock_t mZeroClock;
	DWORD mStartTick;
	LONGLONG mLastTime;
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mFrequency;
	unsigned long mDelta;
	DWORD_PTR mTimerMask;
	std::vector<TimerObject> mTimerEventList;
	std::vector<TimerObject> mTimerEventOnceList;
};

#endif // !__TIMER_H__
