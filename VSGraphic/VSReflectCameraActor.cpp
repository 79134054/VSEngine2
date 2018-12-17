#include "VSReflectCameraActor.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSWorld.h"
#include "VSViewFamily.h"
#include "VSLine3.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSReflectCameraActor, VSActor)
BEGIN_ADD_PROPERTY(VSReflectCameraActor, VSActor)
REGISTER_PROPERTY(m_pTargetCameraActor, TargetCameraActor, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSReflectCameraActor)
IMPLEMENT_INITIAL_END
VSReflectCameraActor::VSReflectCameraActor()
{

}
VSReflectCameraActor::~VSReflectCameraActor()
{

}
void VSReflectCameraActor::Update(double dAppTime)
{
	VSCameraActor::Update(dAppTime);

	if (m_pTargetCameraActor)
	{
		VSCamera * pTargetCamera = m_pTargetCameraActor->GetTypeNode();
		VSCamera * pCamera = GetTypeNode();
		if (pCamera && pTargetCamera)
		{
			pCamera->SetPerspectiveFov(pTargetCamera->GetFov(), pTargetCamera->GetAspect(), pTargetCamera->GetZNear(), pTargetCamera->GetZFar());

			VSVector3 TargetCameraWorldPos =  pTargetCamera->GetWorldTranslate();

			VSVector3 N = m_Plane.GetN();

			VSLine3 Line1(TargetCameraWorldPos, N);

			float t1 = 0;
			m_Plane.RelationWith(Line1, false, t1);

			VSVector3 k1 = Line1.GetParameterPoint(t1);

			VSVector3 CameraWorldPos = k1 + k1 - TargetCameraWorldPos;

			VSVector3 Dir, Up, Right;
			pTargetCamera->GetWorldDir(Dir, Up, Right);


			VSVector3 CameraLookDir = m_Plane.ReflectDir(Dir);
			CameraLookDir.Normalize();

			VSVector3 CameraUp = m_Plane.ReflectDir(Up);
			CameraUp.Normalize();
			
			pCamera->CreateFromLookDirWorld(CameraWorldPos, CameraLookDir, CameraUp);
		}
	}
	
}
void VSReflectCameraActor::SetTargetCameraActor(VSCameraActor * pCameraActor)
{
	m_pTargetCameraActor = DynamicCast<VSReflectCameraActor>(pCameraActor);
	if (m_pTargetCameraActor == NULL)
	{
		m_pTargetCameraActor = pCameraActor;
	}
}
void VSReflectCameraActor::SetPlane(VSPlane3 Plane)
{
	m_Plane = Plane;
	AddCustomCullPlane(m_Plane);
}