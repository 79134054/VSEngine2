#ifndef VSINDIRECTRENDERPASS_H
#define VSINDIRECTRENDERPASS_H
#include "VSPass.h"

namespace VSEngine2
{
	class VSGRAPHIC_API VSIndirectRenderPass : public VSPass
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		VSIndirectRenderPass();
		virtual ~VSIndirectRenderPass();
	public:

		FORCEINLINE void AddLight(VSLight * pLight)
		{
			if (pLight)
			{
				for (unsigned int i = 0 ; i < m_Light.GetNum() ;i++)
				{
					if (m_Light[i] == pLight)
					{
						return ;
					}
				}
				m_Light.AddElement(pLight);
			}
		}

		virtual unsigned int GetPassType()
		{
			return PT_INDIRECT;
		}
	protected:
		VSArray<VSLight *> m_Light;
	public:
		virtual bool Draw(VSRenderer * pRenderer);


	};
	DECLARE_Ptr(VSIndirectRenderPass);
	VSTYPE_MARCO(VSIndirectRenderPass);
}
#endif