#ifndef VSTEXALLSTATE_H
#define VSTEXALLSTATE_H
#include "VSTexture.h"
#include "VSResource.h"
#include "VSSamplerState.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTexAllState : public VSObject , public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

	public:
		VSTexAllState(VSTexture * Texture);
		virtual ~VSTexAllState();
		VSTexAllState();
		virtual unsigned int GetResourceType()const
		{
			return RT_TEXTURE;
		}
	

		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		VSTexturePtr		m_pTex;
	protected:
		VSSamplerDesc		m_SamplerDesc;
		VSSamplerStatePtr	m_pSamplerState;
		bool				m_bSRGB;
	protected:
		static VSPointer<VSTexAllState> Default;
		static VSPointer<VSTexAllState> ms_pOrenNayarLookUpTable;
		static VSPointer<VSTexAllState> ms_pStraussFLookUpTable;
		static VSPointer<VSTexAllState> ms_pStraussSLookUpTable;
		static VSPointer<VSTexAllState> ms_pCookTorranceLookUpTable;
		static VSPointer<VSTexAllState> ms_pIsotropicWardLookUpTable;
	public:
		friend class VSMaterial;
		friend class VSRenderer;
		FORCEINLINE bool GetSRGB()const
		{
			return m_bSRGB;
		}
		void SetSRGBEable(bool bEnable);
		static const VSTexAllState *GetDefalut()
		{
			return Default;
		}
		static const VSTexAllState * GetOrenNayarLookUpTable()
		{
			return ms_pOrenNayarLookUpTable;
		}
		static const VSTexAllState * GetStraussFLookUpTable()
		{
			return ms_pStraussFLookUpTable;
		}
		static const VSTexAllState * GetStraussSLookUpTable()
		{
			return ms_pStraussSLookUpTable;
		}
		static const VSTexAllState * GetCookTorranceTable()
		{
			return ms_pCookTorranceLookUpTable;
		}
		static const VSTexAllState * GetIsotropicWardLookUpTable()
		{
			return ms_pIsotropicWardLookUpTable;
		}
		void SetSamplerState(VSSamplerState * pSamplerState)
		{
			if (!pSamplerState)
			{
				pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
			}
			if (m_pSamplerState != pSamplerState)
			{
				m_pSamplerState = pSamplerState;
				m_SamplerDesc = pSamplerState->GetSamplerDesc();
			}
		}
		const VSSamplerState * GetSamplerState()const
		{
			return m_pSamplerState;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(VSObject * pObjectSrc);
	};
	DECLARE_Ptr(VSTexAllState);
	DECLARE_Proxy(VSTexAllState);
	VSTYPE_MARCO(VSTexAllState);
}
#endif