#ifndef VSMONSTERACTOR1_H
#define VSMONSTERACTOR1_H
#include "VSActor.h"
#include "VSCamera.h"
#include "VSSkelectonMeshComponent.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSMonsterActor1 : public VSActor
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSMonsterActor1();
		~VSMonsterActor1();
		virtual void CreateDefaultComponentNode();
		virtual void ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z);
		void AddCanSeeSence(VSArray<VSString> & SceneMapName);
		virtual void Update(double dAppTime);
	protected:
		VSSkelectonMeshComponent *	m_pMonsterMC;
		VSCamera *					m_pThirdCamera;
		VSWindowViewFamily	*		m_pViewFamily;
		VSREAL						m_fMoveSpeed;
		bool m_bLMouseDowning;
		int		m_iDetMouseX;

		int		m_iLastMouseX;
		VSREAL						m_RotYDelta;
		void ChangeAnimState();
		VSREAL						m_AttackTime;
		bool						m_bAttack;
		VSREAL						m_fAttackStartTime;
	};
	DECLARE_Ptr(VSMonsterActor1);
	VSTYPE_MARCO(VSMonsterActor1);
}
#endif