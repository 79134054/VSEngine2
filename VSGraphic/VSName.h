#ifndef VSNAME_H
#define VSNAME_H
#include "VSString.h"
#include "VSReference.h"
#include "VSPriority.h"
#include "VSCustomArchiveObject.h"
#include "VSInitial.marc"
namespace VSEngine2
{
	class VSGRAPHIC_API VSName : public VSReference,public VSMemObject
	{
	public:
		friend class VSResourceManager;
		~VSName();
		explicit VSName(const TCHAR * pChar,unsigned int uiID);
		explicit VSName(const VSString & String,unsigned int uiID);

		FORCEINLINE const TCHAR * GetBuffer()const
		{
			return m_String.GetBuffer();
		}
		FORCEINLINE const VSString & GetString()const
		{
			return m_String;
		}
		FORCEINLINE unsigned int GetLength()const
		{
			return m_String.GetLength();
		}

		FORCEINLINE unsigned int GetID()const
		{
			return m_uiID;
		}
		
	protected:
		
		unsigned int m_uiID;
		VSString  m_String;
	
	};
	DECLARE_Ptr(VSName);

	class VSStream;
	class VSGRAPHIC_API VSUsedName : public VSCustomArchiveObject
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		~VSUsedName();
		VSUsedName();
		/*explicit */VSUsedName(const TCHAR * pChar);
		/*explicit */VSUsedName(const VSString & String);

		//重载=操作符
		void operator =(const VSString &String);
		void operator =(const TCHAR *pChar);
		void operator =(const VSUsedName & Name);
		FORCEINLINE const TCHAR * GetBuffer()const
		{
			if (!m_pName)
			{
				return NULL;
			}
			return m_pName->GetBuffer();
		}
		FORCEINLINE const VSString & GetString()const
		{
			if (!m_pName)
			{
				return VSString::ms_StringNULL;
			}
			return m_pName->GetString();
		}
		FORCEINLINE unsigned int GetLength()const
		{
			if (!m_pName)
			{
				return 0;
			}
			return m_pName->GetLength();
		}
		FORCEINLINE unsigned int GetNameCode()const
		{
			if (!m_pName)
			{
				return 0;
			}
			return m_pName->GetID();
		}

	
		VSGRAPHIC_API friend bool operator >(const VSUsedName &Name1,const VSUsedName &Name2);
	
		VSGRAPHIC_API friend bool operator <(const VSUsedName &Name1,const VSUsedName &Name2);

		//重载==操作符
		VSGRAPHIC_API friend bool operator ==(const VSUsedName &Name1,const VSUsedName &Name2);

		//重载!=操作符
		VSGRAPHIC_API friend bool operator !=(const VSUsedName &Name1,const VSUsedName &Name2);

		//重载==操作符
		VSGRAPHIC_API friend bool operator ==(const VSUsedName &Name,const VSString & String);

		//重载!=操作符
		VSGRAPHIC_API friend bool operator !=(const VSUsedName &Name,const VSString & String);

		//重载==操作符
		VSGRAPHIC_API friend bool operator ==(const VSUsedName &Name,const TCHAR * pChar);

		//重载!=操作符
		VSGRAPHIC_API friend bool operator !=(const VSUsedName &Name,const TCHAR * pChar);

		virtual void Archive(VSStream & Stream);
		virtual void CopyFrom(VSCustomArchiveObject *,VSMap<VSObject *,VSObject*>& CloneMap);
	protected:
		VSNamePtr m_pName;
	
		DECLARE_INITIAL_ONLY
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		static VSUsedName ms_cPassID;
		static VSUsedName ms_cPrezBeUsedBone;
		static VSUsedName ms_cMaterialVertexFormat;
		static VSUsedName ms_cVolumeVertexFormat;
		static VSUsedName ms_cLightFunKey;
		static VSUsedName ms_cMaterialLightKey;
		static VSUsedName ms_cLighted;

		static VSUsedName ms_cMaterialVertex;
		static VSUsedName ms_cPrezVertex;
		static VSUsedName ms_cNormalDepthVertex;
		static VSUsedName ms_cCubShadowVertex;
		static VSUsedName ms_cVolumeShadowVertex;
		static VSUsedName ms_cShadowVertex;
		static VSUsedName ms_cDualParaboloidShadowVertex;
		static VSUsedName ms_cLightFunVertex;
		static VSUsedName ms_cPostEffectVertex;

		static VSUsedName ms_cCubShadowFov;

		static VSUsedName ms_cPrezPiexl;
		static VSUsedName ms_cPostGray;
		static VSUsedName ms_cGammaCorrect;
		static VSUsedName ms_cPostScreenQuad;
		static VSUsedName ms_cPostVolumeShadowMap;

		static VSUsedName ms_cPostInputTexture;
		static VSUsedName ms_cPostInv_Width;
		static VSUsedName ms_cPostInv_Height;
		
		static VSUsedName ms_cFontColor;

		static VSUsedName ms_cMainScene;
		static VSUsedName ms_cPointLightShadowNum;
		static VSUsedName ms_cPointLightVolumeShadowNum;
		static VSUsedName ms_cPointLightParaboloidShadowNum;
		static VSUsedName ms_cDirectionLightShadowNum;
		static VSUsedName ms_cDirectionLightVolumeShadowNum;
		static VSUsedName ms_cDirectionLightCSMShadowNum;
		static VSUsedName ms_cSpotLightShadowNum;

		// shader use
		static VSUsedName ms_cBlinnPhong;
		static VSUsedName ms_cOrenNayarLookUpTable;
		static VSUsedName ms_cStraussLookUpTable;
		static VSUsedName ms_cCookTorranceSpecluarType;
		static VSUsedName ms_cIsotropicWarLookUpTable;

		static VSUsedName ms_cProjectShadowPreVertex;
		static VSUsedName ms_cProjectShadowPrePixel;

		static VSUsedName ms_cProjectShadowVertex;
		static VSUsedName ms_cProjectShadowPixel;
	};
	CUSTOMTYPE_MARCO(VSUsedName)
}
#endif