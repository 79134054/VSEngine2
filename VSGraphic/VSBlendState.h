#ifndef VSBLENDSTATE_H
#define VSBLENDSTATE_H
#include "VSBind.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSBlendDesc : public VSObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum
		{
			MAX_RENDER_TARGET_NUM = 8
		};
		enum //Blend Parameter
		{
			BP_ZERO,
			BP_ONE,
			BP_SRCCOLOR,
			BP_INVSRCCOLOR,
			BP_SRCALPHA,
			BP_INVSRCALPHA,
			BP_DESTALPHA,
			BP_INVDESTALPHA,
			BP_DESTCOLOR,
			BP_INVDESTCOLOR,
			BP_MAX
		};
		enum// Blend OP
		{
			BO_ADD,
			BO_SUBTRACT,
			BO_REVSUBTRACT,
			BO_MIN_SRC_DEST,
			BO_MAX_SRC_DEST, 
			BO_MAX

		};
		enum //Write Mask
		{
			WM_NONE	 = 0,
			WM_ALPHA = BIT(0),
			WM_RED = BIT(1),
			WM_Green = BIT(2),
			WM_BLUE = BIT(3),
			WM_ALL = 0X0F
		};
		VSBlendDesc()
		{
			bAlphaToCoverageEnable = false;
			bIndependentBlendEnable = false;
			for (unsigned int i = 0 ; i < MAX_RENDER_TARGET_NUM ;i++)
			{
				bBlendEnable[i] = false;
				ucSrcBlend[i] = BP_ONE;
				ucDestBlend[i] = BP_ZERO;
				ucBlendOp[i] = BO_ADD;

				bAlphaBlendEnable[i] = false;
				ucSrcBlendAlpha[i] = BP_ONE;
				ucDestBlendAlpha[i] = BP_ZERO;
				ucBlendOpAlpha[i] = BO_ADD;
				ucWriteMask[i] = WM_ALL;
			}
// 			fBlendColor[0] = fBlendColor[1] = fBlendColor[2] = fBlendColor[3] = 0.0f;
// 			ucSampleMask = 0xffffffff;
		}
		~VSBlendDesc()
		{

		}
		bool			bAlphaToCoverageEnable;
		bool			bIndependentBlendEnable;
// 		VSREAL			fBlendColor[4];
// 		unsigned int	ucSampleMask;
		bool			bBlendEnable[MAX_RENDER_TARGET_NUM];
		unsigned char   ucSrcBlend[MAX_RENDER_TARGET_NUM];
		unsigned char   ucDestBlend[MAX_RENDER_TARGET_NUM];
		unsigned char	ucBlendOp[MAX_RENDER_TARGET_NUM];

		bool			bAlphaBlendEnable[MAX_RENDER_TARGET_NUM];
		unsigned char   ucSrcBlendAlpha[MAX_RENDER_TARGET_NUM];
		unsigned char	ucDestBlendAlpha[MAX_RENDER_TARGET_NUM];
		unsigned char	ucBlendOpAlpha[MAX_RENDER_TARGET_NUM];
		unsigned char   ucWriteMask[MAX_RENDER_TARGET_NUM];
		bool IsBlendUsed(unsigned int uiChannal = 0)const
		{
			return (bBlendEnable[uiChannal] || bAlphaBlendEnable[uiChannal]);
		}
		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(VSBlendDesc)-sizeof(VSObject);
			return (void *)&bAlphaToCoverageEnable;
		}
	};
	VSTYPE_MARCO(VSBlendDesc);
	class VSGRAPHIC_API VSBlendState : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~VSBlendState();
	protected:
		VSBlendState();
		VSBlendDesc m_BlendDesc;
	protected:
		static VSPointer<VSBlendState> Default;
	public:
		static const VSBlendState *GetDefault()
		{
			return Default;
		}
	public:
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		FORCEINLINE const VSBlendDesc & GetBlendDesc()const
		{
			return m_BlendDesc;
		}
		friend class VSResourceManager;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
	};
	DECLARE_Ptr(VSBlendState);
	VSTYPE_MARCO(VSBlendState);
}
#endif