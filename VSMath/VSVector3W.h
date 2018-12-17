#ifndef VSVECTOR3W_H
#define VSVECTOR3W_H
#include "VSMath.h"
#include "VSVector3.h"
namespace VSEngine2
{
	class VSMatrix3X3W;
	//4ÏòÁ¿ ÑÕÉ«
	class  VSMATH_API VSVector3W 
	{
	public:
		union
		{
			VSREAL m[4];
			struct  
			{
				VSREAL x, y, z, w;
			};
			struct  
			{
				VSREAL r,g,b,a;
			};
		};


		VSVector3W(void);
		VSVector3W(VSREAL _x, VSREAL _y, VSREAL _z,VSREAL _w = 1.0f);
		VSVector3W(const VSVector3 & V);

		VSVector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A = 255);
		FORCEINLINE void GetV3(VSVector3 & V)const;
		FORCEINLINE VSVector3 GetV3()const;
		FORCEINLINE void Set(const VSVector3 & V);
		FORCEINLINE void Set(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w = 1.0f);    
		VSVector3W operator * (const VSMatrix3X3W &m)const; 
		
		void operator += (const VSVector3W &v);          
		void operator -= (const VSVector3W &v);          
		void operator *= (const VSVector3W &v);          
		void operator /= (const VSVector3W &v); 

		void operator *= (VSREAL f);                     
		void operator /= (VSREAL f);                     
		void operator += (VSREAL f);                     
		void operator -= (VSREAL f);                     

		

		VSVector3W operator * (VSREAL f)const;            
		VSVector3W operator / (VSREAL f)const;            
		VSVector3W operator + (VSREAL f)const;            
		VSVector3W operator - (VSREAL f)const;            


		VSVector3W operator + (const VSVector3W &v)const; 
		VSVector3W operator - (const VSVector3W &v)const;
		VSVector3W operator * (const VSVector3W &v)const;
		VSVector3W operator / (const VSVector3W &v)const;

		bool operator ==(const VSVector3W &v)const;

		DWORD GetDWARGB()const;
		DWORD GetDWRGBA()const;

		DWORD GetDWBGRA()const;
		DWORD GetDWABGR()const;
		void GetUCColor(unsigned char &R,unsigned char &G,unsigned char &B,unsigned char &A)const;

		void CreateFromARGB(DWORD ARGB);
		void CreateFromBGRA(DWORD BGRA);
		void CreateFromRGBA(DWORD RGBA);
		void CreateFormABGR(DWORD ABGR);
	};  // class

	typedef class VSVector3W VSColorRGBA;
	FORCEINLINE  DWORD VSDWCOLORARGB(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
	}
	FORCEINLINE  DWORD VSDWCOLORBGRA(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((b)&0xff)<<24)|(((g)&0xff)<<16)|(((r)&0xff)<<8)|((a)&0xff)));
	}
	FORCEINLINE  DWORD VSDWCOLORRGBA(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)));
	}
	FORCEINLINE  DWORD VSDWCOLORABGR(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)));
	}

	FORCEINLINE  void VSDWCOLORGetARGB(DWORD ARGB,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		a = (ARGB>>24) & 0xff;
		r = (ARGB>>16) & 0xff;
		g = (ARGB>>8) & 0xff;
		b = (ARGB) & 0xff;
	}
	FORCEINLINE  void VSDWCOLORGetBGRA(DWORD BGRA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		b = (BGRA>>24) & 0xff;
		g = (BGRA>>16) & 0xff;
		r = (BGRA>>8) & 0xff;
		a = (BGRA) & 0xff;
	}
	FORCEINLINE  void VSDWCOLORGetRGBA(DWORD RGBA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		r = (RGBA>>24) & 0xff;
		g = (RGBA>>16) & 0xff;
		b = (RGBA>>8) & 0xff;
		a = (RGBA) & 0xff;
	}
	FORCEINLINE  void VSDWCOLORGetABGR(DWORD ABGR,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		a = (ABGR>>24) & 0xff;
		b = (ABGR>>16) & 0xff;
		g = (ABGR>>8) & 0xff;
		r = (ABGR) & 0xff;
	}
	#include "VSVector3W.inl"
}
#endif