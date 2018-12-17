#ifndef  VSCURVESUBDIVISIONBYUNIFORM_H
#define  VSCURVESUBDIVISIONBYUNIFORM_H
#include "VSCurve3Subdivision.h"
/*
���Ȼ��ַ�



*/
namespace VSEngine2
{
class VSMATH_API VSCurve3SubdivisionByUniform :public VSCurve3Subdivision
{
public:
	VSCurve3SubdivisionByUniform(unsigned int PointNum,VSREAL t1 = 0.0f,VSREAL t2 = 1.0f);
	virtual ~VSCurve3SubdivisionByUniform();
	//������ĸ����������� t1,t2֮��
	virtual bool Subdivision();
	void SetPointNum(unsigned int uiPointNum);
	virtual unsigned int GetSubdivisionMethod(){ return SM_UNIFORM;}
};


}
#endif