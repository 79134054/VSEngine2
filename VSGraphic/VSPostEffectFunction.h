#ifndef VSPOSTEFFECTFUNCTION_H
#define VSPOSTEFFECTFUNCTION_H
#include "VSObject.h"
#include "VSPutNode.h"
#include "VSName.h"
#include "VSSceneRender.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSPostEffectFunction : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~VSPostEffectFunction() = 0;
		friend class VSPostEffectSet;
	protected:
		VSUsedName m_ShowName;
		VSPostEffectFunction(const VSUsedName & ShowName,VSPostEffectSet * pPostEffectSet);
		VSPostEffectFunction();
		VSArray<VSInputNode *> m_pInput;
		VSArray<VSOutputNode *> m_pOutput;
		VSPostEffectSet * m_pOwner;
		bool m_bIsVisited;
		bool m_bLastOne;
		FORCEINLINE void SetOwner(VSPostEffectSet * pOwner)
		{
			VSMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}



		VSPostEffectSceneRenderPtr m_pPostEffectRender;

		virtual VSPostEffectSceneRender * CreateSceneRender() = 0;
		virtual void OnDraw(VSCuller & Culler,double dAppTime) = 0;
	public:
		friend class VSPostEffectSet;
		

		bool ClearFlag();
		

		VSInputNode * GetInputNode(unsigned int uiNodeID)const;
		VSInputNode * GetInputNode(const VSString & NodeName)const;

		VSOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		VSOutputNode * GetOutputNode(const VSString & NodeName)const;

		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		virtual VSRenderTarget * GetMainColorOutPutRenderTarget()
		{
			if (m_pOutput.GetNum() > 0)
			{
				if (m_pOutput[0])
				{
					return GetRenderTarget(m_pOutput[0]);
				}
			}
			return NULL;
		}
		virtual VSRenderTarget * GetMainColorInputRenderTarget()
		{
			if (m_pInput.GetNum() > 0)
			{
				if (m_pInput[0])
				{
					return GetRenderTarget(m_pInput[0]);
				}
			}
			return NULL;
		}
		//取自己节点
		virtual VSRenderTarget * GetRenderTarget(const VSOutputNode * pPutNode)
		{
			for (unsigned int i = 0 ; i < m_pOutput.GetNum() ;i++)
			{
				if (pPutNode == m_pOutput[i])
				{
					return m_pPostEffectRender->GetRenderTarget(i);
				}
			}
			return NULL;
		}
		virtual bool SetEndTarget(VSRenderTarget *pEndTarget)
		{
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					VSPostEffectFunction *pPostEffectFunc = (VSPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if (pPostEffectFunc)
					{
						if (pPostEffectFunc->m_bEnable)
						{
							pPostEffectFunc->CreateSceneRender();
							pPostEffectFunc->m_pPostEffectRender->AddRenderTarget(pEndTarget);
							pPostEffectFunc->m_bLastOne = true;
							return true;
						}
						else
						{
							return pPostEffectFunc->SetEndTarget(pEndTarget);
						}

					}
				}
			}
			return false;
		}
		//取链接的那个节点
		virtual VSRenderTarget * GetRenderTarget(const VSInputNode * pPutNode)
		{
			if(pPutNode->GetOutputLink())
			{
				VSPostEffectFunction *pPostEffectFunc = (VSPostEffectFunction *)pPutNode->GetOutputLink()->GetOwner();
				if (pPostEffectFunc)
				{
					if (pPostEffectFunc->m_bEnable)
					{
						VSRenderTarget * pRenderTarget = pPostEffectFunc->GetRenderTarget(pPutNode->GetOutputLink());
						return pRenderTarget;
					}
					else
					{
						return pPostEffectFunc->GetMainColorInputRenderTarget();
					}
					
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight)
		{
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					VSPostEffectFunction *pPostEffectFunc = (VSPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if(pPostEffectFunc)
					{
						pPostEffectFunc->GetRT(uiWidth,uiHeight);
					}
				}
			}
			
			CreateSceneRender();
			if (m_pPostEffectRender && !m_bLastOne && m_bEnable)
			{
				m_pPostEffectRender->GetRT(uiWidth,uiHeight);
			}
			
		}
		virtual void DisableRT()
		{
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					VSPostEffectFunction *pPostEffectFunc = (VSPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if(pPostEffectFunc)
					{
						pPostEffectFunc->DisableRT();
					}
				}
			}

			CreateSceneRender();
			if (m_pPostEffectRender && !m_bLastOne && m_bEnable)
			{
				m_pPostEffectRender->DisableRT();
			}
		}
		virtual void Draw(VSCuller & Culler,double dAppTime)
		{
			if (m_bIsVisited == 1)
			{
				return ;
			}
			m_bIsVisited = 1;
			
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					VSPostEffectFunction *pPostEffectFunc = (VSPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if(pPostEffectFunc)
					{
						pPostEffectFunc->Draw(Culler,dAppTime);
					}
				}
			}
			if (m_bEnable)
			{
				CreateSceneRender();
				OnDraw(Culler,dAppTime);
			}
			
		}
	public:
		bool m_bEnable;
	};
	DECLARE_Ptr(VSPostEffectFunction);
	VSTYPE_MARCO(VSPostEffectFunction);

}
#endif