#ifndef VSB_SPLINESURFACE3_H
#define VSB_SPLINESURFACE3_H
#include "VSControlSurface3.h"
namespace VSEngine2
{
class VSMATH_API VSB_SplineSurface3 : public VSControlSurface3
{
public:
	VSB_SplineSurface3();
	virtual ~VSB_SplineSurface3();
	bool Copy(const VSB_SplineSurface3 & B_SplineSurface3);
	bool Set(const VSVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn);

	bool SetUKnotVector(const VSREAL * pUKnotVector,unsigned int uiUDegree,unsigned int uiRow);
	bool SetVKnotVector(const VSREAL * pVKnotVector,unsigned int uiVDegree,unsigned int uiColumn);

	bool CreateOpenUniformUKnotVector(unsigned int uiDegree,unsigned int uiRow);
	bool CreatePeriodicUKnotVector(unsigned int uiDegree,unsigned int uiRow);

	bool CreateOpenUniformVKnotVector(unsigned int uiDegree,unsigned int uiColumn);
	bool CreatePeriodicVKnotVector(unsigned int uiDegree,unsigned int uiColumn);
	int GetUKey(VSREAL t)const;
	int GetVKey(VSREAL t)const;

	virtual VSVector3 GetPoint(VSREAL U,VSREAL V) = 0;
	virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V) = 0;
	virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V) = 0;
public:
	/***************************************FORCEINLINE************************************/
	FORCEINLINE bool SetUKnotVector(unsigned int i,VSREAL Value);
	FORCEINLINE const VSREAL * GetpUKnotVector()const;
	FORCEINLINE VSREAL GetUKnotVectorValue(unsigned int i)const;

	FORCEINLINE bool SetVKnotVector(unsigned int i,VSREAL Value);
	FORCEINLINE const VSREAL * GetpVKnotVector()const;
	FORCEINLINE VSREAL GetVKnotVectorValue(unsigned int i)const;
protected:
	FORCEINLINE void SetUBasisValue(int i, int j, VSREAL Value);
	FORCEINLINE VSREAL GetUBasisValue(int i ,int j)const;

	FORCEINLINE void SetUBasisFDValue(int i, int j, VSREAL Value);
	FORCEINLINE VSREAL GetUBasisFDValue(int i ,int j)const;


	FORCEINLINE unsigned int GetUDegree()const;
	FORCEINLINE unsigned int GetUKnotNum()const;


	FORCEINLINE void SetVBasisValue(int i, int j, VSREAL Value);
	FORCEINLINE VSREAL GetVBasisValue(int i ,int j)const;

	FORCEINLINE void SetVBasisFDValue(int i, int j, VSREAL Value);
	FORCEINLINE VSREAL GetVBasisFDValue(int i ,int j)const;


	FORCEINLINE unsigned int GetVDegree()const;
	FORCEINLINE unsigned int GetVKnotNum()const;

protected:
	bool GetUFuncValue(VSREAL t,int &i,unsigned int uiOrder);
	bool GetVFuncValue(VSREAL t,int &i,unsigned int uiOrder);

	VSREAL		* m_pUKnotVector;
	unsigned int m_uiUDegree;
	unsigned int m_uiUKnotNum;
	VSREAL		*m_pUBasis;
	VSREAL		*m_pUBasisFD;
	unsigned int m_uiUBasisNum;


	VSREAL		* m_pVKnotVector;
	unsigned int m_uiVDegree;
	unsigned int m_uiVKnotNum;
	VSREAL		*m_pVBasis;
	VSREAL		*m_pVBasisFD;
	unsigned int m_uiVBasisNum;
};
#include "VSB_SplineSurface3.inl"
}
#endif
