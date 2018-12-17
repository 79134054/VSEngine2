#ifndef VSVERTEXBUFFER_H
#define VSVERTEXBUFFER_H
#include "VSObject.h"
#include "VSBind.h"
#include "VSDataBuffer.h"
#include "VSMath.h"
#include "VSVertexFormat.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSVertexBuffer : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSVertexFormat;
		friend class VSResourceManager;
		VSVertexBuffer(bool bIsStatic);
		VSVertexBuffer(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray,unsigned int uiNum);
		virtual ~VSVertexBuffer();
		bool SetDate(VSDataBuffer * pDate,unsigned int uiVF);
		FORCEINLINE VSDataBuffer * GetDate(unsigned int uiVF,unsigned int uiLevel)const;
		FORCEINLINE unsigned int GetLevel(unsigned int uiVF)const;

		FORCEINLINE VSDataBuffer * GetPositionDate(unsigned int uiLevel)const;
		FORCEINLINE VSDataBuffer * GetNormalDate(unsigned int uiLevel)const;
		FORCEINLINE VSDataBuffer * GetPSizeDate()const;
		FORCEINLINE VSDataBuffer * GetColorDate(unsigned int uiLevel)const;
		FORCEINLINE VSDataBuffer * GetBlendWeightDate()const;
		FORCEINLINE VSDataBuffer * GetBlendIndicesDate()const;
		FORCEINLINE VSDataBuffer * GetTangentDate()const;
		FORCEINLINE VSDataBuffer * GetBinormalDate()const;
		FORCEINLINE VSDataBuffer * GetFogDate()const;
		FORCEINLINE VSDataBuffer * GetDepthDate()const;
		FORCEINLINE VSDataBuffer * GetTexCoordDate(unsigned int uiLevel)const;

		FORCEINLINE unsigned int GetPositionLevel()const;
		FORCEINLINE unsigned int GetColorLevel()const;
		FORCEINLINE unsigned int GetNormalLevel()const;
		FORCEINLINE unsigned int GetTexCoordLevel()const;

		FORCEINLINE unsigned int GetVertexNum()const;

		bool GetVertexFormat(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray);

		FORCEINLINE VSVertexFormat * GetVertexFormat()const;

		FORCEINLINE unsigned int GetOneVertexSize()const;

		FORCEINLINE bool GetLockDateOffSet(unsigned int uiVF,unsigned int uiLevel,unsigned int &uiOffSet)const;
		FORCEINLINE void *GetLockDate(unsigned int uiVF,unsigned int uiLevel)const;

		FORCEINLINE void *GetLockPositionDate(unsigned int uiLevel)const;
		FORCEINLINE void *GetLockNormalDate(unsigned int uiLevel)const;
		FORCEINLINE void *GetLockPSizeDate()const;
		FORCEINLINE void *GetLockColorDate(unsigned int uiLevel)const;
		FORCEINLINE void *GetLockBlendWeightDate()const;
		FORCEINLINE void *GetLockBlendIndicesDate()const;
		FORCEINLINE void *GetLockTangentDate()const;
		FORCEINLINE void *GetLockBinormalDate()const;
		FORCEINLINE void *GetLockFogDate()const;
		FORCEINLINE void *GetLockDepthDate()const;
		FORCEINLINE void *GetLockTexCoordDate(unsigned int uiLevel)const;

		
		virtual unsigned int GetByteSize()const;

		unsigned int GetSemanticsNum(unsigned int uiSemantics)const;

		FORCEINLINE bool HavePositionInfo(unsigned int uiLevel)const;
		FORCEINLINE bool HaveNormalInfo(unsigned int uiLevel)const;
		FORCEINLINE bool HavePSizeInfo()const;
		FORCEINLINE bool HaveColorInfo(unsigned int uiLevel)const;
		FORCEINLINE bool HaveBlendWeightInfo()const;
		FORCEINLINE bool HaveBlendIndicesInfo()const;
		FORCEINLINE bool HaveTangentInfo()const;
		FORCEINLINE bool HaveBinormalInfo()const;
		FORCEINLINE bool HaveFogInfo()const;
		FORCEINLINE bool HaveDepthInfo()const;
		FORCEINLINE bool HaveTexCoordInfo(unsigned int uiLevel)const;

		unsigned int GetSemanticsChannel(unsigned int uiSemantics,unsigned int uiLevel)const;

		FORCEINLINE unsigned int PositionChannel(unsigned int uiLevel)const;
		FORCEINLINE unsigned int NormalChannel(unsigned int uiLevel)const;
		FORCEINLINE unsigned int PSizeChannel()const;
		FORCEINLINE unsigned int ColorChannel(unsigned int uiLevel)const;
		FORCEINLINE unsigned int BlendWeightChannel()const;
		FORCEINLINE unsigned int BlendIndicesChannel()const;
		FORCEINLINE unsigned int TangentChannel()const;
		FORCEINLINE unsigned int BinormalChannel()const;
		FORCEINLINE unsigned int FogChannel()const;
		FORCEINLINE unsigned int DepthChannel()const;
		FORCEINLINE unsigned int TexCoordChannel(unsigned int uiLevel)const;

		unsigned int GetSemanticsDateType(unsigned int uiSemantics,unsigned int uiLevel)const;

		FORCEINLINE unsigned int NormalDateType(unsigned int uiLevel)const;
		FORCEINLINE unsigned int TangentDateType()const;
		FORCEINLINE unsigned int BinormalDateType()const;

	protected:
		friend class VSVertexFormat;
		friend class VSGeometry;
		VSVertexBuffer();
		VSArray<VSDataBufferPtr> m_pDate[VSVertexFormat::VF_MAX];
		unsigned int m_uiVertexNum;
		unsigned int m_uiOneVertexSize;

		VSVertexFormatPtr m_pVertexFormat;
		
		void * m_pLockDate;
		//都要重新实现
	public:
	

		virtual	bool LoadResource(VSRenderer * pRender);

		void *Lock();
		void UnLock();
		FORCEINLINE void * GetLockDatePtr()const
		{
			return m_pLockDate;
		}
		virtual void ClearInfo();
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);

		
	};
	DECLARE_Ptr(VSVertexBuffer);
	VSTYPE_MARCO(VSVertexBuffer);
	#include "VSVertexBuffer.inl"
	
}
#endif