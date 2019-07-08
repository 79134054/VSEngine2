#ifndef VSCURVE3SUBDIVISION_H
#define VSCURVE3SUBDIVISION_H
#include "VSControlCurve3.h"
/*
���߻��ַ���

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
	//��������
	bool SetSubCurve(VSControlCurve3 * pCurve);
    //�õ����ֺ�ĵ�
	FORCEINLINE VSVector3 *GetPoints()const;
	////�õ����ֺ�ĵ����
	FORCEINLINE unsigned int GetPointNum()const;
	//�õ�����
	FORCEINLINE VSControlCurve3 *GetCurve()const;
	//�õ����ֺ����ߵ�ʱ��
	FORCEINLINE VSREAL *GetT()const;
	//ָ�����ߵ�ĳ�����Ƶ�仯�����������л��ֺ�ĵ�
	bool ControlPointChange(unsigned int i);
	//��������
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