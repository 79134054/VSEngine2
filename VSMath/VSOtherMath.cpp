#include "VSOtherMath.h"
namespace VSEngine2
{
	bool IsUniformScale(VSREAL fScale)
	{
		if ( ABS(fScale - 1.0f) < EPSILON_E4)
		{
			return true;
		}
		return false;
	}
	bool IsZeroTranslate(const VSVector3 & Translate)
	{
		if(Translate.GetSqrLength() < EPSILON_E4)
		{
			return true;
		}
		return false;
	}
	bool IsIdentityRotate(const VSMatrix3X3 & Rotate)
	{
		for (unsigned int i = 0 ; i < 3 ; i++)
		{
			for (unsigned int j = 0 ; j < 3 ; j++)
			{
				if (i != j)
				{
					if (ABS(Rotate.M[i][j]) > EPSILON_E4)
					{
						return false;
					}
				}
				else
				{
					if (ABS(Rotate.M[i][j] - 1.0f) > EPSILON_E4)
					{
						return false;
					}
				}

			}
		}
		return true;
	}
	bool IsIdentityRotate(const VSQuat & Rotate)
	{
		VSVector3 R(Rotate.x,Rotate.y,Rotate.z);
		if (R.GetSqrLength() < EPSILON_E4)
		{
			return true;
		}
		return false;
	}
	VSREAL LineInterpolation (VSREAL t1, VSREAL t2, VSREAL t)
	{
		return t1 + (t2 - t1) * t;
	}
	VSVector3 LineInterpolation(const VSVector3& t1, const VSVector3& t2, VSREAL t)
	{
		return t1 + (t2 - t1) * t;
	}
	VSQuat LineInterpolation(const VSQuat& t1, const VSQuat& t2, VSREAL t)
	{
		VSREAL fCos = t2.Dot(t1);
		VSQuat Temp = t1;
		if(fCos < 0.0f)
		{
			Temp = t1 * (-1.0f);
		}
		return Temp + (t2 - Temp) * t;
	}
	VSVector3W LineInterpolation(const VSVector3W& t1, const VSVector3W& t2, VSREAL t)
	{
		return t1 + (t2 - t1) * t;
	}
}
