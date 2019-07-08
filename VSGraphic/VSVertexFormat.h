#ifndef VSVERTEXFORMAT_H
#define VSVERTEXFORMAT_H
#include "VSObject.h"
#include "VSBind.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSVertexFormat : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSVertexFormat();
		virtual ~VSVertexFormat();
		
		enum
		{
			VF_POSITION,
			VF_TEXCOORD,
			VF_NORMAL,
			VF_TANGENT,
			VF_BINORMAL,
			VF_PSIZE,
			VF_COLOR,
			VF_FOG,
			VF_DEPTH,
			VF_BLENDWEIGHT,
			VF_BLENDINDICES,
			VF_MAX
		};
		struct VERTEXFORMAT_TYPE
		{
		public:
			VERTEXFORMAT_TYPE()
			{
				OffSet = 0;
				DateType = 0;
				Semantics = 0;
				SemanticsIndex = 0;
			}
			~VERTEXFORMAT_TYPE()
			{

			}
			UINT OffSet;
			UINT DateType;
			UINT Semantics;
			UINT SemanticsIndex;
		};
	public:
		friend class VSRenderer;
		friend class VSDX9Renderer;

		friend class VSVShaderKey;
		friend class VSPShaderKey;
		friend class VSResourceManager;
		friend class VSShaderKey;

	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(VSResourceIdentifier *pID);
		unsigned int m_uiVertexFormatCode;
	public:
		
		VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;
		FORCEINLINE unsigned int GetVertexFormatCode()const
		{
			return m_uiVertexFormatCode;
		}

	};
	DECLARE_Ptr(VSVertexFormat);
	VSTYPE_MARCO(VSVertexFormat);

}
#endif