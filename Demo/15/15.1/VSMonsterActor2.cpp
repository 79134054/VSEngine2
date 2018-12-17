#include "VSMonsterActor2.h"
#include "VSGraphicInclude.h"
#include "VSViewFamily.h"
#include "VSWorld.h"
#include "VSEngineInput.h"
#include "VSTimer.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMonsterActor2, VSActor)
BEGIN_ADD_PROPERTY(VSMonsterActor2, VSActor)
REGISTER_PROPERTY(m_pMonsterMC, MonsterMC, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pThirdCamera, ThirdCamera, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMonsterActor2)
IMPLEMENT_INITIAL_END
VSMonsterActor2::VSMonsterActor2()
{
	m_pMonsterMC = NULL;
	m_pThirdCamera = NULL;
	m_fMoveSpeed = 0.0f;

	m_bLMouseDowning = false;

	m_iDetMouseX = 0;
	m_RotYDelta = 0.0f;
	m_iLastMouseX = 0;

	m_fAttackStartTime = 0.0f;
	m_AttackTime = 0.9f;
	m_bAttack = false;
}
VSMonsterActor2::~VSMonsterActor2()
{
	m_pMonsterMC = NULL;
	m_pThirdCamera = NULL;
	
}
void VSMonsterActor2::CreateDefaultComponentNode()
{
	VSActor::CreateDefaultComponentNode();

	m_pMonsterMC = AddComponentNode<VSSkelectonMeshComponent>();
	VSResourceProxyBase * pResouce = VSResourceManager::LoadResource(_T("AnimTreeMonster2.SKMODEL"), false);
	if (pResouce)
	{
		unsigned int ResourceType = pResouce->GetResourceType();
		if (ResourceType == VSResource::RT_SKELECTON_MODEL)
		{
			m_pMonsterMC->SetSkelectonMeshResource((VSSkelectonMeshNodeR *)pResouce);
		}
	}
	m_pThirdCamera = AddComponentNode<VSCamera>();
	VSVector3 CameraPos(0.0f, 400.0f, 300.0f);
	VSVector3 CameraDir(0.0f, -1.0f, -1.0f);
	m_pThirdCamera->CreateFromLookDir(CameraPos, CameraDir);
	unsigned int ScreenWidth = VSRenderer::ms_pRenderer->GetScreenWith();
	unsigned int ScreenHeight = VSRenderer::ms_pRenderer->GetScreenWith();
	m_pThirdCamera->SetPerspectiveFov(AngleToRadian(90.0f), (ScreenWidth * 1.0f) / (ScreenHeight), 1.0f, 8000.0f);
	m_pViewFamily = VS_NEW VSWindowViewFamily(_T("WindowUse"), m_pThirdCamera, NULL, VSForwordEffectSceneRenderMethod::ms_Type.GetName().GetBuffer(), -1);

}
void VSMonsterActor2::Update(double dAppTime)
{
	VSMatrix3X3 Rotator = GetWorldRotate();
	VSVector3 U, V, N;
	Rotator.GetUVN(U, V, N);
	VSVector3 FrontDir = N * (-1.0f);

	VSVector3 Pos = GetWorldPos();
	Pos += FrontDir * m_fMoveSpeed * (VSREAL)VSTimer::ms_pTimer->GetDetTime();
	SetWorldPos(Pos);

	if (m_bLMouseDowning)
	{
		m_RotYDelta = ((m_iDetMouseX)* 1.0f) * (VSREAL)VSTimer::ms_pTimer->GetDetTime();
		m_iDetMouseX = 0;

	}


	VSMatrix3X3 RotY;
	RotY.CreateRotY(m_RotYDelta);
	SetWorldRotate(Rotator * RotY);

	ChangeAnimState();
}
void VSMonsterActor2::ChangeAnimState()
{
	VSREAL ParaFinal = 0.0f;
	VSREAL ParaIdle_Walk = 0.0f;

	if (m_bAttack == true)
	{
		ParaFinal = 1.0f;
	}
	else
	{
		ParaFinal = 0.0f;
	}

	if (m_fMoveSpeed > 0.01f)
	{
		ParaIdle_Walk = 1.0f;
	}
	else
	{
		ParaIdle_Walk = 0.0f;
	}
	if (m_bAttack == true)
	{
		if (m_fAttackStartTime >= m_AttackTime)
		{
			m_bAttack = false;
		}
		m_fAttackStartTime += (VSREAL)VSTimer::ms_pTimer->GetDetTime();

	}
	static VSUsedName Final = _T("Final");
	m_pMonsterMC->SetAnimTreeNodePara(Final, &ParaFinal);
	static VSUsedName Idle_Walk = _T("Idle_Walk");
	m_pMonsterMC->SetAnimTreeNodePara(Idle_Walk, &ParaIdle_Walk);
}
void VSMonsterActor2::ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z)
{
	VSActor::ProcessInput(uiInputType, uiEvent, uiKey, x, y, z);
	
	if (uiInputType == VSEngineInput::IT_KEYBOARD)
	{
		if (uiEvent == VSEngineInput::IE_DOWN)
		{
			if (uiKey == VSEngineInput::BK_UP)
			{
				m_fMoveSpeed = 120.0f;			
			}
			else if (uiKey == VSEngineInput::BK_A)
			{
				m_bAttack = true;
				m_fAttackStartTime = 0.0f;
			}
			else if (uiKey == VSEngineInput::BK_LEFT)
			{
				m_RotYDelta = -0.004f;
			}
			else if (uiKey == VSEngineInput::BK_RIGHT)
			{
				m_RotYDelta = 0.004f;
			}
		}
		else if (uiEvent == VSEngineInput::IE_UP)
		{
			if (uiKey == VSEngineInput::BK_UP)
			{
				m_fMoveSpeed = 0.0f;			
			}
			else if (uiKey == VSEngineInput::BK_LEFT)
			{
				m_RotYDelta = 0.0f;
			}
			else if (uiKey == VSEngineInput::BK_RIGHT)
			{
				m_RotYDelta = 0.0f;
			}
		}
	}
	else if (uiInputType == VSEngineInput::IT_MOUSE)
	{
		if (uiEvent == VSEngineInput::IE_DOWN)
		{
			if (uiKey == VSEngineInput::MK_RIGHT)
			{

			}
			else if (uiKey == VSEngineInput::MK_LEFT)
			{
				m_bLMouseDowning = true;
				m_iLastMouseX = x;

				m_iDetMouseX = 0;

			}
			else if (uiKey == VSEngineInput::MK_MIDDLE)
			{
			}
		}
		else if (uiEvent == VSEngineInput::IE_UP)
		{
			if (uiKey == VSEngineInput::MK_RIGHT)
			{

			}
			else if (uiKey == VSEngineInput::MK_LEFT)
			{
				m_bLMouseDowning = false;
				m_RotYDelta = 0.0f;
			}
			else if (uiKey == VSEngineInput::MK_MIDDLE)
			{
			}
		}
		else if (uiEvent == VSEngineInput::IE_MOUSE_MOVE)
		{
			if (m_bLMouseDowning)
			{
				m_iDetMouseX = x - m_iLastMouseX;

				m_iLastMouseX = x;

			}
		}
	}
	return;

}
void VSMonsterActor2::AddCanSeeSence(VSArray<VSString> & SceneMapName)
{
	if (SceneMapName.GetNum() > 0)
	{
		for (unsigned int i = 0; i < SceneMapName.GetNum(); i++)
		{
			VSSceneMap * pSceneMap = VSWorld::ms_pWorld->GetScene(SceneMapName[i]);

			m_pViewFamily->AddScene(pSceneMap->GetScene());
		}
	}
	else
	{
		VSSceneMap * pSceneMap = VSWorld::ms_pWorld->GetScene(VSUsedName::ms_cMainScene);
		m_pViewFamily->AddScene(pSceneMap->GetScene());

	}
}
