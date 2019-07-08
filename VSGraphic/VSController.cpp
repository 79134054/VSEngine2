#include "VSController.h"
#include "VSMath.h"
#include "VSStream.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSController,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSController)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSController,VSObject)
REGISTER_PROPERTY(m_bEnable,Enable,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiRepeatType,uiRepeatType,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_dMinTime,MinTime,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_dMaxTime,MaxTime,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_dPhase,Phase,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_dFrequency,Frequency,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pObject,pObject,VSProperty::F_SAVE_LOAD_COPY);
REGISTER_PROPERTY(m_bIsVisibleUpdate,IsVisibleUpdate,VSProperty::F_SAVE_LOAD_COPY);
REGISTER_PROPERTY(m_TimeEventArray, TimeEventArray, VSProperty::F_SAVE_LOAD_COPY);
END_ADD_PROPERTY
VSController::VSController()
{
	m_uiRepeatType = RT_NONE;
	m_dMinTime = 0.0;
	m_dMaxTime = 0.0;
	m_dPhase = 0.0;
	m_dFrequency = 1.0;
	m_pObject = NULL;
	m_dLastAppTime = 0;
	m_dIntervalTime = 0.0;
	m_dIntervalAnimTime = 0;
	m_dLastAnimTime = 0;
	m_bStart = 0;
	m_dStartSystemTime = 0;
	m_dNowAnimTime = 0.0;
	m_bEnable = true;
	m_bIsVisibleUpdate = true;
	m_fInnerTimeScale = 1.0f;
	AddTimeEvent(VS_NEW VSStartAnimEvent(this));
	AddTimeEvent(VS_NEW VSEndAnimEvent(this));
}
VSController::~VSController()
{
	m_pObject = NULL;
}
bool VSController::UpdateEx(double aAppTime)
{
	return 1;
}
void VSController::SetEnable(bool Enable)
{
	m_bEnable = Enable;
	if (!m_bEnable)
	{
		m_TriggerStop();
	}
}
bool VSController::Update(double dAppTime)
{
	if(!m_bEnable)
	{
		m_bStart = 0;
		return 0;
	}
	
	if(!m_bStart)
	{
		m_dStartSystemTime = dAppTime;
		m_bStart = 1;
		m_dLastAppTime = 0;
		m_dTimeSum = 0;
		m_dLastAnimTime = 0.0f;
		m_TriggerBeginStart();
	}

	dAppTime = dAppTime - m_dStartSystemTime;
	m_dIntervalTime = ABS(dAppTime - m_dLastAppTime);
	m_dIntervalAnimTime = m_dIntervalTime * m_dFrequency;
	m_dLastAppTime = dAppTime;
	dAppTime = GetControlTime(dAppTime);

	m_dNowAnimTime = dAppTime;
	
	m_dTimeSum += m_dIntervalTime;

	TimeEvent(dAppTime);
	UpdateEx(dAppTime);
	
	
	m_dLastAnimTime = dAppTime;
	return 1;
}
void VSController::TimeEvent(double dAppTime)
{
	for (unsigned int i = 0; i < m_TimeEventArray.GetNum(); i++)
	{
		if (!m_TimeEventArray[i] || !m_TimeEventArray[i]->m_bEnable)
		{
			continue;
		}
		double dRange = m_dMaxTime - m_dMinTime;
		double RealTime = dRange * m_TimeEventArray[i]->m_fTriggerPercent;


		if (m_uiRepeatType == RT_WRAP)
		{
			if (m_dLastAnimTime + m_dIntervalAnimTime > m_dMaxTime)
			{
				if (RealTime >= m_dLastAnimTime || RealTime <= dAppTime)
				{
					m_TimeEventArray[i]->Trigger();
				}
			}
			else
			{
				if (RealTime >= m_dLastAnimTime && RealTime <= dAppTime)
				{
					m_TimeEventArray[i]->Trigger();
				}
			}

		}
		else if (m_uiRepeatType == RT_CYCLE)
		{
			if (m_dLastAnimTime + m_dIntervalAnimTime > m_dMaxTime)
			{
				if (RealTime >= m_dLastAnimTime || RealTime >= dAppTime)
				{
					m_TimeEventArray[i]->Trigger();
				}
			}
			else if (m_dLastAnimTime - m_dIntervalAnimTime < m_dMinTime)
			{
				if (RealTime <= m_dLastAnimTime || RealTime <= dAppTime)
				{
					m_TimeEventArray[i]->Trigger();
				}
			}
			else
			{
				double TempMax = Max(m_dLastAnimTime, dAppTime);
				double TempMin = Min(m_dLastAnimTime, dAppTime);
				if (RealTime >= TempMin && RealTime <= TempMax)
				{
					m_TimeEventArray[i]->Trigger();
				}
			}
		}
		else
		{
			if (RealTime >= m_dLastAnimTime && RealTime <= dAppTime)
			{
				m_TimeEventArray[i]->Trigger();
			}
		}

	}
}
double VSController::GetControlTime(double dAppTime)
{
	double dCtrlTime = m_dFrequency * dAppTime * m_fInnerTimeScale + m_dPhase;
	if(m_uiRepeatType == RT_NONE)
	{
		return dCtrlTime;
	}
	if (m_uiRepeatType == RT_CLAMP)
	{
		if (dCtrlTime < m_dMinTime)
		{
			return m_dMinTime;
		}
		if (dCtrlTime > m_dMaxTime)
		{
			return m_dMaxTime;
		}
		return dCtrlTime;
	}

	double dRange = m_dMaxTime - m_dMinTime;
	if (dRange > 0.0)
	{
		double dMultiples = (dCtrlTime - m_dMinTime) / dRange;
		double dIntTime = floor(dMultiples);
		double dFrcTime = dMultiples - dIntTime;
		if (m_uiRepeatType == RT_WRAP)
		{
			return m_dMinTime + dFrcTime * dRange;
		}
		if(m_uiRepeatType == RT_CYCLE)
		{
			if (int(dIntTime) & 1)
			{
				// backward time
				return m_dMaxTime - dFrcTime * dRange;

			}
			else
			{
				// forward time
				return m_dMinTime + dFrcTime*  dRange;
				
			}
		}
		
		return m_dMinTime;
	}
	else
	{
		return m_dMinTime;
	}

}
bool VSController::SetObject(VSObject * pObject)
{
	if(!pObject)
		return 0;
	m_pObject = pObject;
	return 1;
}
double VSController::GetTime(double fTime, double fMin,double fMax,unsigned int uiRepeatType)
{
	if(uiRepeatType == RT_NONE || uiRepeatType >= RT_MAX)
	{
		return fTime;
	}
	if (uiRepeatType == RT_CLAMP)
	{
		if (fTime < fMin)
		{
			return fMin;
		}
		if (fTime > fMax)
		{
			return fMax;
		}
		return fTime;
	}

	double dRange = fMax - fMin;
	if (dRange > 0.0)
	{
		double dMultiples = (fTime - fMin) / dRange;
		double dIntTime = floor(dMultiples);
		double dFrcTime = dMultiples - dIntTime;
		if (uiRepeatType == RT_WRAP)
		{
			return fMin + dFrcTime * dRange;
		}
		if(uiRepeatType == RT_CYCLE)
		{
			if (int(dIntTime) & 1)
			{
				// backward time
				return fMax - dFrcTime * dRange;

			}
			else
			{
				// forward time
				return fMin + dFrcTime*  dRange;

			}
		}

		return fMin;
	}
	else
	{
		return fMin;
	}
}
void VSController::AddTimeEvent(VSTimeEvent * pTimeEvent)
{
	if (!pTimeEvent)
	{
		return;
	}
	m_TimeEventArray.AddElement(pTimeEvent);
}
void VSController::DeleteTimeEvent(VSTimeEvent * pTimeEvent)
{
	if (!pTimeEvent)
	{
		return;
	}
	unsigned int Index = m_TimeEventArray.FindElement(pTimeEvent);
	if (Index >= m_TimeEventArray.GetNum())
	{
		return ;
	}
	else
	{
		m_TimeEventArray.Erase(Index);
	}
}
void VSController::AddTriggerStart(TriggerAnimEventType::Handler handler)
{
	m_TimeEventArray[0]->m_TriggerAnimEvent += handler;
}

void VSController::AddTriggerStop(TriggerAnimEventType::Handler handler)
{
	m_TriggerStop += handler;
}

void VSController::AddTriggerEnd(TriggerAnimEventType::Handler handler)
{
	m_TimeEventArray[1]->m_TriggerAnimEvent += handler;
}

void VSController::AddTriggerBeginStart(TriggerAnimEventType::Handler handler)
{
	m_TriggerBeginStart += handler;
}
IMPLEMENT_RTTI(VSStartAnimEvent, VSTimeEvent)
BEGIN_ADD_PROPERTY(VSStartAnimEvent, VSTimeEvent)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStartAnimEvent)
IMPLEMENT_INITIAL_END
VSStartAnimEvent::VSStartAnimEvent(VSController *pController)
:VSTimeEvent(pController)
{
	m_fTriggerPercent = 0.0f;
}
VSStartAnimEvent::VSStartAnimEvent()
{
	m_fTriggerPercent = 0.0f;
}
VSStartAnimEvent::~VSStartAnimEvent()
{

}
IMPLEMENT_RTTI(VSEndAnimEvent, VSTimeEvent)
BEGIN_ADD_PROPERTY(VSEndAnimEvent, VSTimeEvent)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSEndAnimEvent)
IMPLEMENT_INITIAL_END
VSEndAnimEvent::VSEndAnimEvent(VSController *pController)
:VSTimeEvent(pController)
{
	m_fTriggerPercent = 1.0f;
}
VSEndAnimEvent::VSEndAnimEvent()
{
	m_fTriggerPercent = 1.0f;
}
VSEndAnimEvent::~VSEndAnimEvent()
{

}
