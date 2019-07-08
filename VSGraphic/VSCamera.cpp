#include "VSCamera.h"
#include "VSRenderer.h"
#include "VSGraphicInclude.h"
#include "VS2DTexture.h"
#include "VSDepthStencil.h"
#include "VSStream.h"
#include "VSSceneManager.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCamera, VSNodeComponent)
BEGIN_ADD_PROPERTY(VSCamera, VSNodeComponent)
REGISTER_PROPERTY(m_RotX,RotX,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotY,RotY,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotZ,RotZ,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Fov, Fov, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Aspect,Aspect,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ZFar, ZFar, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ZNear, ZNear, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ViewPort, ViewPort, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ViewMat,ViewMat,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ProjMat,ProjMat,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_CustomCullPlane, CustomCullPlane, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCamera)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSCamera> VSCamera::Default;
bool VSCamera::InitialDefaultState()
{
	VSCamera *p = NULL;
	p = VS_NEW VSCamera();

	if(p)
	{
		Default = p;
		
	}
	else
		return 0;

	return 1;
}
bool VSCamera::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}

VSCamera::VSCamera()
{
	m_ViewMat.Identity();
	m_ProjMat.Identity();
	m_RotX = 0.0;						
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Fov = (VSREAL)0.8;
	m_Aspect =(VSREAL)(4.0 / 3.0);
	m_ZNear= (VSREAL)1.0;
	m_ZFar = (VSREAL)8000.0;
	m_ProjMat.CreatePerspective(m_Fov,m_Aspect,m_ZFar,m_ZNear);
	m_bEnable = true;

}
VSCamera::~VSCamera()
{
	for (unsigned int i = 0 ; i < m_ViewFamilyArray.GetNum() ; i++)
	{
		VSSceneManager::ms_pSceneManager->DeleteViewFamily(m_ViewFamilyArray[i]);
		VSMAC_DELETE(m_ViewFamilyArray[i]);
	}

}
void VSCamera::CreateFromEuler(const VSVector3 &Pos,VSREAL RotX,VSREAL RotY , VSREAL RotZ)
{
	SetLocalTranslate(Pos);
	m_RotX = RotX;
	m_RotY = RotY;
	m_RotZ = RotZ;
	VSQuat    qFrame(0,0,0,1);			

	qFrame.CreateEuler(m_RotY,m_RotX,  m_RotZ);

	VSMatrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);
	SetLocalRotate(Mat);

}
void VSCamera::CreateFromLookDir(const VSVector3 &Pos,
					   const VSVector3 &vcDir,
					   const VSVector3 &vcUp)
{

	VSMatrix3X3W MatTemp;
	MatTemp.CreateFromLookDir(Pos,vcDir,vcUp);

	VSMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);
	VSMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);

	MatInv.GetEluer(m_RotZ,m_RotX,m_RotY);
	SetLocalRotate(MatInv);
	SetLocalTranslate(Pos);
}
void VSCamera::CreateFromLookAt(const VSVector3 &vcPos,									
					  const VSVector3 &vcLookAt,							
					  const VSVector3 &vcUp)
{
	VSMatrix3X3W MatTemp;
	MatTemp.CreateFromLookAt(vcPos,vcLookAt,vcUp);

	VSMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);

	VSMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);

	MatInv.GetEluer(m_RotZ,m_RotX,m_RotY); 
	SetLocalRotate(MatInv);
	SetLocalTranslate(vcPos);
}
void VSCamera::CreateFromLookDirWorld(const VSVector3 &Pos,
	const VSVector3 &vcDir,
	const VSVector3 &vcUp)
{

	VSMatrix3X3W MatTemp;
	MatTemp.CreateFromLookDir(Pos, vcDir, vcUp);

	VSMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);
	VSMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);

	SetWorldRotate(MatInv);
	SetWorldTranslate(Pos);
	m_Local.GetRotate().GetEluer(m_RotZ,m_RotX,  m_RotY);
}
void VSCamera::CreateFromLookAtWorld(const VSVector3 &vcPos,
	const VSVector3 &vcLookAt,
	const VSVector3 &vcUp)
{
	VSMatrix3X3W MatTemp;
	MatTemp.CreateFromLookAt(vcPos, vcLookAt, vcUp);

	VSMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);

	VSMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);
	SetWorldRotate(MatInv);
	SetWorldTranslate(vcPos);
	m_Local.GetRotate().GetEluer(m_RotZ,m_RotX,  m_RotY);
}
bool VSCamera::SetAspect(VSREAL Aspect)
{
	m_Aspect = Aspect;
	return m_ProjMat.CreatePerspective(m_Fov,m_Aspect,m_ZNear,m_ZFar);
}
bool VSCamera::SetPerspectiveFov(VSREAL fFov ,				//X方向张角
					   VSREAL Aspect,			//宽高比
					   VSREAL fZN ,		//近剪裁面
					   VSREAL fZF)		//远剪裁面
{

	m_Fov = fFov;
	m_Aspect = Aspect;
	m_ZFar = fZF;
	m_ZNear = fZN;
	return m_ProjMat.CreatePerspective(m_Fov,m_Aspect,m_ZNear,m_ZFar);
}
bool VSCamera::SetOrthogonal(VSREAL fW ,				//宽
				   VSREAL fH,					//高
				   VSREAL fZN ,				//近剪裁面
				   VSREAL fZF)				//远剪裁面
{
	m_ZFar = fZF;
	m_ZNear = fZN;
	return m_ProjMat.CreateOrthogonal(fW,fH,m_ZNear,m_ZFar);
}
/*----------------------------------------------------------------*/
/*
相机体平面求法

假设ViewProj为相机矩阵和投影矩阵的乘积
所以世界坐标下(x,y,z,1)任意一点经过这个矩阵变换后为(x',y',z',1)

(x,y,z,1)ViewProj = (x',y',z',1)''''''''''''''''''''''''''''''''''(1)

经过变换后的点的范围-1 <= x <= 1 , -1 <= y <= 1 ,0 <= z <= 1
也就是相机体的由棱台变成了长方体,共6个面,我们让法向量朝外
left面为-x-1 = 0( x + 1 = 0 )
near面为 -z = 0
其他面同理

假设平面位于世界坐标下的方程为ax + by + cz + d = 0

				a	
				b
则有(x,y,z,1)(	c	) = 0				'''''''''''''''''''''''''(2)
				d

假设平面变换后的方程为a'x + b'y + c'z + d' = 0

					a'	
					b'
则有(x',y',z',1)(	c'	) = 0			''''''''''''''''''''''''''(3)
					d'
(3)和(1)结合:

						a'
						b'
导出(x,y,z,1)ViewProj(	c'	) = 0		'''''''''''''''''''''''''''''(4)
						d'
(4)和(2)导出

	a					a'
	b					b'
(	c	) = ViewProj(	c'	)
	d					d'

所有平面的投影方程都知道a',b',c',d'知道),就能求出a,b,c,d
*/
void VSCamera::GetPlane(VSPlane3 Plane[VSCamera::CP_MAX])const
{
	VSMatrix3X3W ViewProj;
	ViewProj = m_ViewMat * m_ProjMat;

	VSVector3 N;
	VSREAL  fD;

	// right plane
	N.x = -(ViewProj._03 - ViewProj._00);
	N.y = -(ViewProj._13 - ViewProj._10);
	N.z = -(ViewProj._23 - ViewProj._20);
	fD    = -(ViewProj._33 - ViewProj._30);
	Plane[0].Set(N,fD);

	// left plane

	N.x = -(ViewProj._03 + ViewProj._00);
	N.y = -(ViewProj._13 + ViewProj._10);
	N.z = -(ViewProj._23 + ViewProj._20);
	fD    = -(ViewProj._33 + ViewProj._30);
	Plane[1].Set(N,fD);
	
	// top plane
	N.x = -(ViewProj._03 - ViewProj._01);
	N.y = -(ViewProj._13 - ViewProj._11);
	N.z = -(ViewProj._23 - ViewProj._21);
	fD    = -(ViewProj._33 - ViewProj._31);
	Plane[2].Set(N,fD);
	// bottom plane
	N.x = -(ViewProj._03 + ViewProj._01);
	N.y = -(ViewProj._13 + ViewProj._11);
	N.z = -(ViewProj._23 + ViewProj._21);
	fD    = -(ViewProj._33 + ViewProj._31);
	Plane[3].Set(N,fD);

	// far plane
	N.x = -(ViewProj._03 - ViewProj._02);
	N.y = -(ViewProj._13 - ViewProj._12);
	N.z = -(ViewProj._23 - ViewProj._22);
	fD    = -(ViewProj._33 - ViewProj._32);
	Plane[4].Set(N,fD);

	// near plane
	N.x = -ViewProj._02;
	N.y = -ViewProj._12;
	N.z = -ViewProj._22;
	fD    = -ViewProj._32;
	Plane[5].Set(N,fD);
	


}
VSAABB3 VSCamera::GetFrustumAABB()
{
	VSVector3 Point[8];
	GetFrustumPoint(Point);

	VSAABB3 Aabb;
	Aabb.CreateAABB(Point,8);
	
	return Aabb;
}
void VSCamera::GetFrustumPoint(VSVector3 Point[8])
{
	VSMatrix3X3W ViewProj = m_ViewMat * m_ProjMat;

	VSMatrix3X3W ViewProjInv = ViewProj.GetInverse();

	Point[0] = VSVector3(1,1,0);
	Point[1] = VSVector3(1,-1,0);
	Point[2] = VSVector3(-1,1,0);
	Point[3] = VSVector3(-1,-1,0);

	Point[4] = VSVector3(1,1,1);
	Point[5] = VSVector3(1,-1,1);
	Point[6] = VSVector3(-1,1,1);
	Point[7] = VSVector3(-1,-1,1);

	for(unsigned int i = 0 ; i < 8 ;i++)
	{
		Point[i] = Point[i] * ViewProjInv;
	}
}
void VSCamera::UpdateCameraState(double dAppTime)
{
	VSNodeComponent::UpdateCameraState(dAppTime);
	m_pAllCamera.AddElement(this);
}
void VSCamera::UpdateTransform(double dAppTime) 
{ 

	VSNodeComponent::UpdateTransform(dAppTime);
	if(m_bIsChanged)
	{
 		VSTransform Trans = GetWorldTransform();
 		m_ViewMat = Trans.GetCombineInverse();
	}
}
void VSCamera::AddViewFamily(VSViewFamily * pViewFamily)
{
	if (pViewFamily)
	{
		m_ViewFamilyArray.AddElement(pViewFamily);
		VSSceneManager::ms_pSceneManager->AddViewFamily(pViewFamily);
	}
}

void VSCamera::DeleteViewFamily(VSViewFamily * pViewFamily)
{
	if (pViewFamily)
	{
		unsigned int uiID = m_ViewFamilyArray.FindElement(pViewFamily);
		if (uiID >= m_ViewFamilyArray.GetNum())
		{
			return ;
		}
		m_ViewFamilyArray.Erase(uiID);
		VSSceneManager::ms_pSceneManager->DeleteViewFamily(pViewFamily);
		VSMAC_DELETE(pViewFamily);
	}
}
void VSCamera::AddCustomCullPlane(const VSPlane3& P)
{
	m_CustomCullPlane.AddElement(P);
}
void VSCamera::ClearCustomCullPlane()
{
	m_CustomCullPlane.Clear();
}