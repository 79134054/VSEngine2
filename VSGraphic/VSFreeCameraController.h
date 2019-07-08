#ifndef VSFREECAMERACONTROLLER_H
#define VSFREECAMERACONTROLLER_H
#include "VSMath.h"
#include "VSVector3.h"
#include "VSController.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSFreeCameraController : public VSController
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSFreeCameraController();
		virtual ~VSFreeCameraController();
		VSREAL m_RotXDelta;
		VSREAL m_RotYDelta;
		VSREAL m_RotZDelta;
		VSREAL m_MoveDelta;
		VSVector3 m_MoveDirection;
		virtual bool UpdateEx(double dAppTime);

	};
	DECLARE_Ptr(VSFreeCameraController);
	VSTYPE_MARCO(VSFreeCameraController);

}
#endif