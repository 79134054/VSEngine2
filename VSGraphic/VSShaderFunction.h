#ifndef  VSSHADERFUNCTION_H
#define  VSSHADERFUNCTION_H
#include "VSObject.h"
#include "VSPutNode.h"
#include "VSName.h"
#include "VSBitArray.h"
namespace VSEngine2
{
	class VSMaterial;
	class VSStream;
	class VSShaderMainFunction;
	class VSGRAPHIC_API VSShaderFunction : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		virtual ~VSShaderFunction() = 0;
	protected:
		VSUsedName m_ShowName;
		VSShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		VSShaderFunction();
		VSArray<VSInputNode *> m_pInput;
		VSArray<VSOutputNode *> m_pOutput;
		VSMaterial * m_pOwner;
		bool m_bIsVisited;
		FORCEINLINE void SetOwner(VSMaterial * pOwner)
		{
			VSMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

	public:
		friend class VSMaterial;
		virtual bool IsHaveOutPut()
		{
			if (m_pOutput.GetNum() > 0)
			{
				return m_pOutput[0]->IsConnection();
			}
			return false;
		}
		virtual void ResetInShaderName() = 0;
		virtual bool GetInputValueString(VSString &OutString)const;
		virtual bool GetOutPutValueString(VSString &OutString)const;
		virtual bool GetFuntionString(VSString &OutString)const = 0;
		bool GetShaderTreeString(VSString &OutString);
		bool ClearShaderTreeStringFlag();
		

		VSInputNode * GetInputNode(unsigned int uiNodeID)const;
		VSInputNode * GetInputNode(const VSString & NodeName)const;
		FORCEINLINE unsigned int GetInputNodeNum()const
		{
			return m_pInput.GetNum();
		}
		VSOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		VSOutputNode * GetOutputNode(const VSString & NodeName)const;
		FORCEINLINE unsigned int GetOutputNodeNum()const
		{
			return m_pOutput.GetNum();
		}
		FORCEINLINE const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}

		VSString GetValueEqualString(const VSOutputNode * pOutPutNode,const VSInputNode * pInputNode)const;


		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction);

		virtual bool CheckChildNodeValidToThis(VSArray<VSShaderFunction *> & NoValidShaderFunctionArray);

		virtual bool GetAllChildNode(VSArray<VSShaderFunction *> & ChildNodeArray);

		virtual bool CheckChildNodeValidAll(VSMap<VSShaderFunction *,VSArray<VSShaderFunction *>> & NoValidMap);

		virtual bool HaveThisChild(VSShaderFunction * pShaderFunction);

		virtual void GetNoLightFunctionParentNode(VSArray<VSShaderFunction *> & ChildNodeArray);

		virtual bool ResetValueType();


	};
	DECLARE_Ptr(VSShaderFunction);
	VSTYPE_MARCO(VSShaderFunction);
}
#endif