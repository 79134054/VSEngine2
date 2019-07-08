#ifndef VSTRANSFORM_H
#define VSTRANSFORM_H
#include "VSVector3.h"
#include "VSMatrix3X3W.h"
#include "VSOtherMath.h"
#include "VSGraphic.h"
//#pragma comment(lib, "VSMath.lib")
/*
	正变换的变换顺序是S,R,T


*/

namespace VSEngine2
{
class VSStream;
class VSGRAPHIC_API VSTransform
{

private:
	VSMatrix3X3W m_mCombine;
	VSMatrix3X3W m_mCombineInverse;
	void Combine();
	VSVector3 m_fScale;
	VSMatrix3X3 m_mRotate;
	VSVector3 m_vTranslate;
	bool m_bIsCombine;
	bool m_bIsHaveInverse;
public:
	
	VSTransform();
	~VSTransform();

	FORCEINLINE const VSMatrix3X3W & GetCombine();
	FORCEINLINE const VSMatrix3X3W & GetCombineInverse();
	FORCEINLINE const VSVector3& GetScale()const;
	FORCEINLINE const VSVector3 & GetTranslate()const;
	FORCEINLINE const VSMatrix3X3 & GetRotate()const;
	FORCEINLINE void GetDir(VSVector3 &Dir,VSVector3 &Up,VSVector3 & Right)const;
	FORCEINLINE void SetScale(const VSVector3& fScale);
	FORCEINLINE void SetTranslate(const VSVector3& Translate);
	FORCEINLINE void SetRotate(const VSMatrix3X3 & Rotate);
	
	enum //Transform type
	{
		TT_POS,
		TT_DIR,
		TT_MAX
	};
	void ApplyForward(const VSVector3 & In, VSVector3 & Out,unsigned int uiTransformType);
	enum //Transform Flag
	{
		TF_SCALE = 1,
		TF_ROTATE = 2,
		TF_TRANSFORM = 4,
		TF_ALL = TF_SCALE | TF_TRANSFORM | TF_ROTATE
	};
	void Product(const VSTransform & t1,const VSTransform &t2,unsigned int TransformFlag = TF_ALL);
	void Inverse(VSTransform & Out)const;
	void ApplyInverse(const VSVector3 & In, VSVector3 & Out,unsigned int uiTransformType);
	
	void SetMatrix(const VSMatrix3X3W & VSMat);

	void Interpolation(const VSTransform &t1 , const VSTransform &t2,VSREAL t);

	void Indetity();
	const static VSTransform ms_Indetity;
};

#include "VSTransform.inl"
};
#endif