#include "VS1stCameraController.h"
#include "VSCamera.h"
#include "VSQuat.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VS1stCameraController,VSController)
BEGIN_ADD_PROPERTY(VS1stCameraController,VSController)
REGISTER_PROPERTY(m_RotXDelta,RotXDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotYDelta,RotYDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveXDelta,MoveXDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveYDelta,MoveYDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveZDelta,MoveZDelta,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS1stCameraController)
IMPLEMENT_INITIAL_END
VS1stCameraController::VS1stCameraController()
{
	m_RotXDelta = 0.0f;
	m_RotYDelta = 0.0f;
	m_MoveXDelta = 0.0f;
	m_MoveYDelta = 0.0f;
	m_MoveZDelta = 0.0f;
}
VS1stCameraController:: ~VS1stCameraController()
{

}
bool VS1stCameraController::UpdateEx(double dAppTime)
{
	if(!VSController::UpdateEx(dAppTime))
		return 0;


	//VSFreeCamera * Temp = (VSFreeCamera *)m_pObject;

	VSCamera* Temp = DynamicCast<VSCamera>(m_pObject);
	if(!Temp)
		return 0;
	Temp->m_RotX += m_RotXDelta * (VSREAL)m_dIntervalTime;
	Temp->m_RotY += m_RotYDelta * (VSREAL)m_dIntervalTime;
	Temp->m_RotZ  = 0;

	if (Temp->m_RotX > 1.4707963f) Temp->m_RotX = 1.4707963f;
	else if (Temp->m_RotX < -1.4707963f) Temp->m_RotX = -1.4707963f;

	if (Temp->m_RotY > VS2PI) Temp->m_RotY -= VS2PI;
	else if (Temp->m_RotY < -VS2PI) Temp->m_RotY += VS2PI;


	VSVector3 LocalPos = Temp->GetLocalTranslate();
	VSMatrix3X3 Rot = Temp->GetLocalRotate();
	VSVector3 Velocity(m_MoveXDelta,m_MoveYDelta,m_MoveZDelta);
	LocalPos += Velocity * Rot * (VSREAL)m_dIntervalTime;

	VSQuat    qFrame(0,0,0,1);			

	qFrame.CreateEuler(Temp->m_RotZ,Temp->m_RotX,Temp->m_RotY );

	VSMatrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);

	Temp->SetLocalRotate(Mat);
	Temp->SetLocalTranslate(LocalPos);

	return 1;
}
