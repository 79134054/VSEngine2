#include "VSFreeCameraController.h"
#include "VSCamera.h"
#include "VSQuat.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSFreeCameraController,VSController)
BEGIN_ADD_PROPERTY(VSFreeCameraController,VSController)
REGISTER_PROPERTY(m_RotXDelta,RotXDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotYDelta,RotYDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotZDelta,RotZDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveDelta,MoveDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveDirection,MoveDirection,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSFreeCameraController)
IMPLEMENT_INITIAL_END
VSFreeCameraController::VSFreeCameraController()
{
	m_RotXDelta = 1.0;
	m_RotYDelta = 1.0;
	m_RotZDelta = 1.0;
	m_MoveDelta = 1.0;
}
VSFreeCameraController:: ~VSFreeCameraController()
{

}
bool VSFreeCameraController::UpdateEx(double dAppTime)
{
	if(!VSController::UpdateEx(dAppTime))
		return 0;


	//VSFreeCamera * Temp = (VSFreeCamera *)m_pObject;

	VSCamera* Temp = DynamicCast<VSCamera>(m_pObject);
	if(!Temp)
		return 0;
	Temp->m_RotX += m_RotXDelta * (VSREAL)m_dIntervalTime;
	Temp->m_RotY += m_RotYDelta * (VSREAL)m_dIntervalTime;
	Temp->m_RotZ += m_RotZDelta * (VSREAL)m_dIntervalTime;
	
	
	if (Temp->m_RotX > VS2PI) Temp->m_RotX -= VS2PI;
	else if (Temp->m_RotX < -VS2PI) Temp->m_RotX += VS2PI;

	if (Temp->m_RotY > VS2PI) Temp->m_RotY -= VS2PI;
	else if (Temp->m_RotY < -VS2PI) Temp->m_RotY += VS2PI;

	if (Temp->m_RotZ > VS2PI) Temp->m_RotZ -= VS2PI;
	else if (Temp->m_RotZ < -VS2PI) Temp->m_RotZ += VS2PI;
	
	VSVector3 LocalPos = Temp->GetLocalTranslate();
 	LocalPos += m_MoveDirection * m_MoveDelta * (VSREAL)m_dIntervalTime;
			
	VSQuat    qFrame(0,0,0,1);	
	qFrame.CreateEuler(Temp->m_RotZ, Temp->m_RotX, Temp->m_RotY);

	VSMatrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);

	Temp->SetLocalRotate(Mat);
	Temp->SetLocalTranslate(LocalPos);
	return 1;
}



