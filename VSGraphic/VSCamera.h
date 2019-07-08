#ifndef  VSCAMERA_H
#define  VSCAMERA_H
#include "VSNodeComponent.h"
#include "VSVector3.h"
#include "VSMatrix3X3.h"
#include "VSMatrix3X3W.h"
#include "VSPlane3.h"

namespace VSEngine2
{
class VSRenderer;
typedef struct VSVIEWPORT_TYPE
{
	VSREAL       XMin;
	VSREAL       YMin;            
	VSREAL       XMax;
	VSREAL       YMax;
	VSREAL		ZMin;
	VSREAL		ZMax;
	bool		bChangeAspect;
	void Reset()
	{
		XMin = 0.0f;
		YMin = 0.0f;
		XMax = 1.0f;
		YMax = 1.0f;
		ZMin = 0.0f;
		ZMax = 1.0f;
		bChangeAspect = false;
	}
	VSVIEWPORT_TYPE()
	{
		Reset();
	}
}VSViewPort;
class VSStream;
class VSGRAPHIC_API VSCamera : public VSNodeComponent
{
	//PRIORITY
	
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	friend class VSViewFamily;
	VSCamera();
	virtual ~VSCamera();
	enum //Camera Plane
	{	
		CP_RIGHT,
		CP_LEFT,
		CP_TOP,
		CP_BOTTOM,
		CP_FAR,
		CP_NEAR,
		CP_MAX = 6
	};
	FORCEINLINE const VSMatrix3X3W & GetViewMatrix()const;
	void CreateFromEuler(const VSVector3 &Pos,VSREAL RotX,VSREAL RotY , VSREAL RotZ);
	void CreateFromLookDir(const VSVector3 &Pos,
							const VSVector3 &vcDir,
							const VSVector3 &vcUp = VSVector3(0,1,0));
	void CreateFromLookAt(const VSVector3 &vcPos,									
							const VSVector3 &vcLookAt,							
							const VSVector3 &vcUp = VSVector3(0,1,0));	
	void CreateFromLookDirWorld(const VSVector3 &Pos,
		const VSVector3 &vcDir,
		const VSVector3 &vcUp = VSVector3(0, 1, 0));
	void CreateFromLookAtWorld(const VSVector3 &vcPos,
		const VSVector3 &vcLookAt,
		const VSVector3 &vcUp = VSVector3(0, 1, 0));
	bool SetPerspectiveFov(VSREAL fFov ,		//X方向张角
						VSREAL Aspect,	//宽高比
						VSREAL fZN ,		//近剪裁面
						VSREAL fZF);		//远剪裁面
	bool SetAspect(VSREAL Aspect);
	
	bool SetOrthogonal(VSREAL fW ,				//宽
						VSREAL fH,					//高
						VSREAL fZN ,				//近剪裁面
						VSREAL fZF)	;				//远剪裁面
	FORCEINLINE const VSMatrix3X3W &GetProjMatrix()const;
	FORCEINLINE bool AddViewPort(const VSViewPort &ViewPort);//视口				
	FORCEINLINE VSViewPort* GetViewPort(unsigned int i)const;
	FORCEINLINE unsigned int GetViewPortNum()const
	{
		return m_ViewPort.GetNum();
	}
	void GetFrustumPoint(VSVector3 Point[8]);
	VSAABB3 GetFrustumAABB();
	void GetPlane(VSPlane3 Plane[VSCamera::CP_MAX])const;
	FORCEINLINE VSREAL GetZFar()const
	{
		return m_ZFar;
	}
	FORCEINLINE VSREAL GetZNear()const
	{
		return m_ZNear;
	}
	FORCEINLINE VSREAL GetAspect()const
	{
		return m_Aspect;
	}
	FORCEINLINE VSREAL GetFov()const
	{
		return m_Fov;
	}
	FORCEINLINE VSViewFamily * GetViewFamily(unsigned int uiID)
	{
		if (uiID < m_ViewFamilyArray.GetNum())
		{
			return m_ViewFamilyArray[uiID];
		}
		return NULL;
	}
	FORCEINLINE unsigned int GetViewFamilyNum()
	{
		return m_ViewFamilyArray.GetNum();
	}
	FORCEINLINE void ClearProject()
	{
		m_ProjMat.Identity();
	}
	void AddCustomCullPlane(const VSPlane3& P);
	void ClearCustomCullPlane();
	VSArray<VSPlane3> m_CustomCullPlane;
	virtual void UpdateCameraState(double dAppTime);
public:
	VSREAL		m_RotX;							//在局部坐标系下的EULER角(旋转角)
	VSREAL		m_RotY;
	VSREAL		m_RotZ;

//	VSVector3	m_LocalPos;						//位置

protected :
	friend class VSFreeCameraController;
	friend class VS1stCameraController;
	friend class VSSpatial;
	friend class VSScene;
	friend class VSSceneManager;
	friend class VSViewFamily;
	friend class VSLight;
	VSArray<VSViewPort>		m_ViewPort;
	VSMatrix3X3W	m_ViewMat;					//相机矩阵
	VSMatrix3X3W	m_ProjMat;
	

	VSREAL		m_Fov;
	VSREAL		m_Aspect;
	VSREAL		m_ZFar;
	VSREAL		m_ZNear;
	VSArrayOrder<VSViewFamily *> m_ViewFamilyArray;
	
	void AddViewFamily(VSViewFamily * pViewFamily);

	
	void DeleteViewFamily(VSViewFamily * pViewFamily);

	
public:
	virtual void UpdateTransform(double dAppTime);//更新变换信息 
public:

	static bool InitialDefaultState();
	static bool TerminalDefaultState();

protected:
	static VSPointer<VSCamera> Default;
	
public:
	friend class VSProjLight;
	static const VSCamera * GetDefault()
	{
		return Default;
	}
};

#include "VSCamera.inl"
DECLARE_Ptr(VSCamera);
VSTYPE_MARCO(VSCamera);
};
#endif