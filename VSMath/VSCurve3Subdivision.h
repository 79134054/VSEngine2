#ifndef VSCURVE3SUBDIVISION_H
#define VSCURVE3SUBDIVISION_H
#include "VSControlCurve3.h"
/*
曲线划分方法

*/
namespace VSEngine2
{
class VSMATH_API VSCurve3Subdivision : public VSMemObject
{
public:
	enum	//Subdivision Method
	{
		SM_ERROR,
		SM_LENGTH,
		SM_UNIFORM,
		SM_MAX
	};
	VSCurve3Subdivision();
	virtual ~VSCurve3Subdivision() = 0;
	//设置曲线
	bool SetSubCurve(VSControlCurve3 * pCurve);
    //得到划分后的点
	FORCEINLINE VSVector3 *GetPoints()const;
	////得到划分后的点个数
	FORCEINLINE unsigned int GetPointNum()const;
	//得到曲线
	FORCEINLINE VSControlCurve3 *GetCurve()const;
	//得到划分后曲线的时间
	FORCEINLINE VSREAL *GetT()const;
	//指定曲线的某个控制点变化，重新求所有划分后的点
	bool ControlPointChange(unsigned int i);
	//划分曲线
	virtual bool Subdivision() = 0;

	virtual unsigned int GetSubdivisionMethod() = 0;
protected:
	VSVector3 *m_pPoint;
	unsigned int m_uiPointNum;
	VSControlCurve3 *m_pCurve;
	VSREAL			*m_pT;
	VSREAL m_ft1;
	VSREAL m_ft2;
};

#include "VSCurve3Subdivision.inl"
}
#endif