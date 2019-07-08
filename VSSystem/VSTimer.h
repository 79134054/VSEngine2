#ifndef VSTIMER_H
#define VSTIMER_H
#include "VSSystem.h"
#include "VSMath.h"
namespace VSEngine2
{
class VSSYSTEM_API VSTimer
{
private:
	bool m_bUseLargeTime;   //使用大时间标志
	 __int64 m_int64OneSecondTicks;  //一秒内的滴答次数
	 __int64 m_int64TimeTickStartCounts;  //开始的滴答计数值
	unsigned long m_ulTimeStart;  //timeGetTime开始时间
	int m_iFrameCount;
	double m_fFPS;
	double m_fTime,m_fLastFPSTime,m_fTimeSlice;
	double m_fDetTime, m_fLastTime;
	void InitGameTime();
public:
	VSTimer();
	~VSTimer();
	
	double GetGamePlayTime();
	void UpdateFPS();
	FORCEINLINE double GetFPS(){return m_fFPS;}
	static VSTimer * ms_pTimer;
	double GetDetTime(){ return m_fDetTime; }
	int GetRandSeed();
};
}
#endif