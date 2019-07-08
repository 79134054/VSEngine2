#include "VSFastFunction.h"
#include "VSVector3.h"
#include "VSVector3W.h"
#include "VSMatrix3X3.h"
#include "VSMatrix3X3W.h"
#include <xmmintrin.h>
namespace VSEngine2
{
void VSFastMul(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSMatrix3X3W & OutM)
{
	__asm
	{
		mov		eax, [InM2]
		mov		ecx, [InM1]
		movups	xmm4, [eax]
		movups	xmm5, [eax+16]
		movups	xmm6, [eax+32]
		movups	xmm7, [eax+48]

		// 1
		movss	xmm0, [ecx]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm4

		movss	xmm1, [ecx+4]
		shufps	xmm1, xmm1, 0
		mulps	xmm1, xmm5

		movss	xmm2, [ecx+8]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm6

		addps	xmm1, xmm0			// Row 1

		movss	xmm3, [ecx+12]
		shufps	xmm3, xmm3, 0
		mulps	xmm3, xmm7

		// 2
		movss	xmm0, [ecx+16]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm4

		addps	xmm3, xmm2			// Row 1

		movss	xmm2, [ecx+20]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm5

		addps	xmm3, xmm1			// Row 1

		mov		eax, [OutM]

		movss	xmm1, [ecx+24]
		shufps	xmm1, xmm1, 0
		mulps	xmm1, xmm6

		movups	[eax], xmm3			// Row 1 out

		addps	xmm2, xmm0			// Row 2

		movss	xmm3, [ecx+28]
		shufps	xmm3, xmm3, 0
		mulps	xmm3, xmm7

		// 3
		movss	xmm0, [ecx+32]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm4

		addps	xmm3, xmm1			// Row 2

		movss	xmm1, [ecx+36]
		shufps	xmm1, xmm1, 0
		mulps	xmm1, xmm5

		addps	xmm3, xmm2			// Row 2

		movss	xmm2, [ecx+40]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm6

		movups	[eax+16], xmm3		// Row 2 out

		addps	xmm1, xmm0			// Row 3

		movss	xmm3, [ecx+44]
		shufps	xmm3, xmm3, 0
		mulps	xmm3, xmm7

		// 4
		movss	xmm0, [ecx+48]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm4

		addps	xmm3, xmm2			// Row 3

		movss	xmm2, [ecx+52]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm5

		addps	xmm3, xmm1			// Row 3

		movss	xmm1, [ecx+56]
		shufps	xmm1, xmm1, 0
		mulps	xmm1, xmm6

		movups	[eax+32], xmm3		// Row 3 out

		addps	xmm2, xmm0			// Row 4

		movss	xmm3, [ecx+60]
		shufps	xmm3, xmm3, 0
		mulps	xmm3, xmm7

		addps	xmm3, xmm1			// Row 4

		addps	xmm3, xmm2			// Row 4

		movups	[eax+48], xmm3		// Row 4 out
	}
}
void VSFastAdd(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSMatrix3X3W & OutM)
{
	__asm
	{
		mov eax, [InM2];
		mov ecx, [InM1];

		movups xmm4, [eax];
		movups xmm5, [eax+16];
		movups xmm6, [eax+32];
		movups xmm7, [eax+48];

		mov eax, [OutM];

		movups xmm0, [ecx];
		movups xmm1, [ecx+16];
		movups xmm2, [ecx+32];
		movups xmm3, [ecx+48];

		addps xmm0, xmm4;
		movups [eax], xmm0;

		addps xmm1, xmm5;
		movups [eax+16], xmm1;

		addps xmm2, xmm6;
		movups [eax+32], xmm2;

		addps xmm3, xmm7;
		movups [eax+48], xmm3;
	}
}
void VSFastSub(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSMatrix3X3W & OutM)
{
	__asm
	{
		mov eax, [InM2];
		mov ecx, [InM1];

		movups xmm4, [eax];
		movups xmm5, [eax+16];
		movups xmm6, [eax+32];
		movups xmm7, [eax+48];

		mov eax, [OutM];

		movups xmm0, [ecx];
		movups xmm1, [ecx+16];
		movups xmm2, [ecx+32];
		movups xmm3, [ecx+48];

		subps xmm0, xmm4;
		movups [eax], xmm0;

		subps xmm1, xmm5;
		movups [eax+16], xmm1;

		subps xmm2, xmm6;
		movups [eax+32], xmm2;

		subps xmm3, xmm7;
		movups [eax+48], xmm3;
	}
}
void VSFastMul(const VSMatrix3X3W & InM1,VSREAL f,VSMatrix3X3W & OutM)
{
	__asm
	{
		mov ecx, [InM1];
		movss xmm0, [f];

		movups xmm4, [ecx];
		movups xmm5, [ecx+16];
		movups xmm6, [ecx+32];
		movups xmm7, [ecx+48];

		mov eax, [OutM];

		shufps xmm0, xmm0, 0;

		mulps xmm4, xmm0;
		movups [eax], xmm4;

		mulps xmm5, xmm0;
		movups [eax+16], xmm5;

		mulps xmm6, xmm0;
		movups [eax+32], xmm6;

		mulps xmm7, xmm0;
		movups [eax+48], xmm7;
	}
}
void VSFastLerp(const VSMatrix3X3W & InM1,const VSMatrix3X3W & InM2,VSREAL f,VSMatrix3X3W & OutM)
{
	__asm
	{
		mov eax, [InM2];
		mov ecx, [InM1];

		movups xmm4, [eax];
		movups xmm5, [eax+16];
		movups xmm6, [eax+32];
		movups xmm7, [eax+48];

		movups xmm0, [ecx];
		movups xmm1, [ecx+16];
		movups xmm2, [ecx+32];
		movups xmm3, [ecx+48];

		subps xmm4, xmm0;

		mov eax, [OutM];

		subps xmm5, xmm1;
		subps xmm6, xmm2;

		movups [eax], xmm5;

		subps xmm7, xmm3;

		movss xmm5, [f];

		shufps xmm5, xmm5, 0;

		mulps xmm4, xmm5;
		mulps xmm6, xmm5;
		addps xmm0, xmm4;
		mulps xmm7, xmm5;
		movups xmm4, [eax];

		mulps xmm4, xmm5;
		movups [eax], xmm0;

		addps xmm2, xmm6;
		addps xmm1, xmm4;
		movups [eax+32], xmm2;
		addps xmm3, xmm7;

		movups [eax+16], xmm1;
		movups [eax+48], xmm3;
	}
}

VSREAL VSFastLength(const VSVector2 &vec)
{
	VSREAL rt;
	const VSREAL fThree = 3.0f;
	const VSREAL fOneHalf = 0.5f;
	__asm
	{
		mov ecx, [vec];
		movups xmm0, [ecx];
		mulps xmm0, xmm0;
		movaps xmm1, xmm0;
		shufps xmm1, xmm1, _MM_SHUFFLE(1, 1, 1, 1);
		addss xmm1, xmm0;
		rsqrtss xmm0, xmm1;
		// Newton-Raphson iteration
		movss xmm3, [fThree];
		movss xmm2, xmm0;
		mulss xmm0, xmm1;
		mulss xmm0, xmm2;
		mulss xmm2, [fOneHalf];
		subss xmm3, xmm0;
		mulss xmm3, xmm2;

		xorps xmm4, xmm4;
		cmpss xmm4, xmm1, 4;

		mulss xmm3, xmm1;
		andps xmm3, xmm4;
		movss [rt], xmm3;
	}
	return rt;
}
VSREAL VSFastLength(const VSVector3 &vec)
{
	VSREAL rt;
	const VSREAL fThree = 3.0f;
	const VSREAL fOneHalf = 0.5f;
	__asm
	{
		mov ecx, [vec];
		movups xmm0, [ecx];
		mulps xmm0, xmm0;

		movaps xmm1, xmm0;
		movaps xmm2, xmm0;
		shufps xmm1, xmm1, _MM_SHUFFLE(1, 1, 1, 1);
		shufps xmm2, xmm2, _MM_SHUFFLE(2, 2, 2, 2);

		addss xmm1, xmm0;
		addss xmm1, xmm2;
		rsqrtss xmm0, xmm1;
		// Newton-Raphson iteration
		movss xmm3, [fThree];
		movss xmm2, xmm0;
		mulss xmm0, xmm1;
		mulss xmm0, xmm2;
		mulss xmm2, [fOneHalf];
		subss xmm3, xmm0;
		mulss xmm3, xmm2;

		xorps xmm4, xmm4;
		cmpss xmm4, xmm1, 4;

		mulss xmm3, xmm1;
		andps xmm3, xmm4;
		movss [rt], xmm3;
	}
	return rt;
}
VSREAL VSFastLength(const VSVector3W &vec)
{
	VSREAL rt;
	const VSREAL fThree = 3.0f;
	const VSREAL fOneHalf = 0.5f;
	__asm
	{
		mov ecx, [vec];
		movups xmm0, [ecx];
		mulps xmm0, xmm0;

		movaps xmm1, xmm0;
		shufps xmm0, xmm0, _MM_SHUFFLE(0, 0, 2, 0);
		shufps xmm1, xmm1, _MM_SHUFFLE(0, 0, 3, 1);

		addps xmm1, xmm0;

		movaps xmm2, xmm1;
		shufps xmm2, xmm2, _MM_SHUFFLE(1, 1, 1, 1);

		addss xmm1, xmm2;

		rsqrtss xmm0, xmm1;
		// Newton-Raphson iteration
		movss xmm3, [fThree];
		movss xmm2, xmm0;
		mulss xmm0, xmm1;
		mulss xmm0, xmm2;
		mulss xmm2, [fOneHalf];
		subss xmm3, xmm0;
		mulss xmm3, xmm2;

		xorps xmm4, xmm4;
		cmpss xmm4, xmm1, 4;

		mulss xmm3, xmm1;
		andps xmm3, xmm4;
		movss [rt], xmm3;
	}
	return rt;
}


void VSFastCross(const VSVector3 &InV1, const VSVector3 &InV2,VSVector3 &OutV)
{
	__asm
	{
		mov ecx, [InV1];
		mov eax, [InV2];

		movups xmm0, [ecx];
		movups xmm1, [eax];

		movaps xmm2, xmm0;
		movaps xmm3, xmm1;

		shufps xmm0, xmm0, _MM_SHUFFLE(0, 0, 2, 1);
		shufps xmm1, xmm1, _MM_SHUFFLE(0, 1, 0, 2);

		mulps xmm0, xmm1;

		mov eax, [OutV];

		shufps xmm2, xmm2, _MM_SHUFFLE(0, 1, 0, 2);
		shufps xmm3, xmm3, _MM_SHUFFLE(0, 0, 2, 1);

		mulps xmm2, xmm3;
		subps xmm0, xmm2;

		movss [eax], xmm0;
		shufps xmm0, xmm0, _MM_SHUFFLE(3, 2, 1, 1);
		movss [eax+4], xmm0;
		shufps xmm0, xmm0, _MM_SHUFFLE(3, 2, 1, 2);
		movss [eax+8], xmm0;
	}
}

void VSFastNormalize(const VSVector2 &InV,VSVector2 &OutV)
{
	const VSREAL fThree = 3.0f;
	const VSREAL fOneHalf = 0.5f;
	__asm
	{
		mov ecx, [InV];
		movups xmm0, [ecx];
		movaps xmm5, xmm0;
		mulps xmm0, xmm0;

		movaps xmm1, xmm0;
		shufps xmm0, xmm0, _MM_SHUFFLE(0, 0, 0, 0);
		shufps xmm1, xmm1, _MM_SHUFFLE(1, 1, 1, 1);

		addss xmm1, xmm0;
		rsqrtss xmm0, xmm1;
		// Newton-Raphson iteration
		movss xmm3, [fThree];
		movss xmm2, xmm0;
		mulss xmm0, xmm1;
		mulss xmm0, xmm2;
		mulss xmm2, [fOneHalf];
		subss xmm3, xmm0;
		mulss xmm3, xmm2;

		xorps xmm4, xmm4;
		cmpss xmm4, xmm1, 4;

		andps xmm3, xmm4;

		shufps xmm3, xmm3, 0;
		mulps xmm3, xmm5;
		mov ecx, [OutV];

		movss [ecx], xmm3;

		shufps xmm3, xmm3, _MM_SHUFFLE(1, 1, 1, 1);
		movss [ecx+4], xmm3;
	}
}
void VSFastNormalize(const VSVector3 &InV,VSVector3 &OutV)
{
	const VSREAL fThree = 3.0f;
	const VSREAL fOneHalf = 0.5f;
	__asm
	{
		mov ecx, [InV];
		movups xmm0, [ecx];
		movaps xmm5, xmm0;
		mulps xmm0, xmm0;

		movaps xmm1, xmm0;
		movaps xmm2, xmm0;
		shufps xmm1, xmm1, _MM_SHUFFLE(1, 1, 1, 1);
		shufps xmm2, xmm2, _MM_SHUFFLE(2, 2, 2, 2);

		addss xmm1, xmm0;
		addss xmm1, xmm2;
		rsqrtss xmm0, xmm1;
		// Newton-Raphson iteration
		movss xmm3, [fThree];
		movss xmm2, xmm0;
		mulss xmm0, xmm1;
		mulss xmm0, xmm2;
		mulss xmm2, [fOneHalf];
		subss xmm3, xmm0;
		mulss xmm3, xmm2;

		xorps xmm4, xmm4;
		cmpss xmm4, xmm1, 4;

		andps xmm3, xmm4;

		shufps xmm3, xmm3, 0;
		mulps xmm3, xmm5;
		mov ecx, [OutV];

		movss [ecx], xmm3;

		shufps xmm3, xmm3, _MM_SHUFFLE(3, 2, 1, 1);
		movss [ecx+4], xmm3;

		shufps xmm3, xmm3, _MM_SHUFFLE(3, 2, 1, 2);
		movss [ecx+8], xmm3;
	}
}
void VSFastNormalize(const VSVector3W &InV,VSVector3W &OutV)
{
	const VSREAL fThree = 3.0f;
	const VSREAL fOneHalf = 0.5f;
	__asm
	{
		mov ecx, [InV];
		movups xmm0, [ecx];
		movaps xmm5, xmm0;
		mulps xmm0, xmm0;

		movaps xmm1, xmm0;

		shufps xmm0, xmm0, _MM_SHUFFLE(0, 0, 2, 0);
		shufps xmm1, xmm1, _MM_SHUFFLE(0, 0, 3, 1);

		addps xmm1, xmm0;

		movaps xmm2, xmm1;
		shufps xmm2, xmm2, _MM_SHUFFLE(1, 1, 1, 1);

		addss xmm1, xmm2;

		rsqrtss xmm0, xmm1;
		// Newton-Raphson iteration
		movss xmm3, [fThree];
		movss xmm2, xmm0;
		mulss xmm0, xmm1;
		mulss xmm0, xmm2;
		mulss xmm2, [fOneHalf];
		subss xmm3, xmm0;
		mulss xmm3, xmm2;

		xorps xmm4, xmm4;
		cmpss xmm4, xmm1, 4;

		andps xmm3, xmm4;

		shufps xmm3, xmm3, 0;
		mulps xmm3, xmm5;
		mov ecx, [OutV];

		movups [ecx], xmm3;
	}
}
}