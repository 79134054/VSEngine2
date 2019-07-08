#include "VSName.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
VSName::~VSName()
{

	m_uiID = 0;
	m_String.Clear();
	
}
VSName::VSName(const TCHAR * pChar,unsigned int uiID)
{
	VSMAC_ASSERT(pChar);
	m_uiID = uiID;
	m_String.Clear();
	m_String = pChar;
}
VSName::VSName(const VSString & String,unsigned int uiID)
{
	m_uiID = uiID;
	m_String.Clear();
	m_String = String;

}

VSUsedName::~VSUsedName()
{
	m_pName = NULL;
}
VSUsedName::VSUsedName()
{
	m_pName = NULL;
}
VSUsedName::VSUsedName(const TCHAR * pChar)
{
	m_pName = NULL;

	m_pName = VSResourceManager::CreateName(pChar);
}
VSUsedName::VSUsedName(const VSString & String)
{
	m_pName = NULL;
	m_pName = VSResourceManager::CreateName(String);
}
void VSUsedName::operator =(const VSString &String)
{
	m_pName = NULL;
	m_pName = VSResourceManager::CreateName(String);
		
}
void VSUsedName::operator =(const TCHAR *pChar)
{
	m_pName = NULL;

	m_pName = VSResourceManager::CreateName(pChar);
}
void VSUsedName::operator =(const VSUsedName & Name)
{
	m_pName = Name.m_pName;
}
VSUsedName VSUsedName::ms_cPrezBeUsedBone;
VSUsedName VSUsedName::ms_cMaterialVertexFormat;
VSUsedName VSUsedName::ms_cVolumeVertexFormat;
VSUsedName VSUsedName::ms_cLightFunKey;
VSUsedName VSUsedName::ms_cMaterialLightKey;
VSUsedName VSUsedName::ms_cLighted;
VSUsedName VSUsedName::ms_cPointLightShadowNum;
VSUsedName VSUsedName::ms_cPointLightVolumeShadowNum;
VSUsedName VSUsedName::ms_cPointLightParaboloidShadowNum;
VSUsedName VSUsedName::ms_cDirectionLightShadowNum;
VSUsedName VSUsedName::ms_cDirectionLightVolumeShadowNum;
VSUsedName VSUsedName::ms_cDirectionLightCSMShadowNum;
VSUsedName VSUsedName::ms_cBlinnPhong;
VSUsedName VSUsedName::ms_cOrenNayarLookUpTable;
VSUsedName VSUsedName::ms_cStraussLookUpTable;
VSUsedName VSUsedName::ms_cMaterialVertex;
VSUsedName VSUsedName::ms_cPrezVertex;
VSUsedName VSUsedName::ms_cPrezPiexl;
VSUsedName VSUsedName::ms_cPostGray;
VSUsedName VSUsedName::ms_cGammaCorrect;
VSUsedName VSUsedName::ms_cPostScreenQuad;
VSUsedName VSUsedName::ms_cPostVolumeShadowMap;
VSUsedName VSUsedName::ms_cPostInputTexture;
VSUsedName VSUsedName::ms_cPostInv_Width;
VSUsedName VSUsedName::ms_cPostInv_Height;
VSUsedName VSUsedName::ms_cNormalDepthVertex;
VSUsedName VSUsedName::ms_cCubShadowVertex;
VSUsedName VSUsedName::ms_cVolumeShadowVertex;
VSUsedName VSUsedName::ms_cShadowVertex;
VSUsedName VSUsedName::ms_cDualParaboloidShadowVertex;
VSUsedName VSUsedName::ms_cPassID;
VSUsedName VSUsedName::ms_cMainScene;
VSUsedName VSUsedName::ms_cCookTorranceSpecluarType;
VSUsedName VSUsedName::ms_cIsotropicWarLookUpTable;
VSUsedName VSUsedName::ms_cCubShadowFov;
VSUsedName VSUsedName::ms_cSpotLightShadowNum;
VSUsedName VSUsedName::ms_cLightFunVertex;
VSUsedName VSUsedName::ms_cPostEffectVertex;
VSUsedName VSUsedName::ms_cProjectShadowPreVertex;
VSUsedName VSUsedName::ms_cProjectShadowPrePixel;
VSUsedName VSUsedName::ms_cProjectShadowVertex;
VSUsedName VSUsedName::ms_cProjectShadowPixel;
VSUsedName VSUsedName::ms_cFontColor;
IMPLEMENT_PRIORITY(VSUsedName)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSUsedName)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool VSUsedName::InitialDefaultState()
{
	ms_cPrezBeUsedBone = _T("PrezBeUsedBone");
	ms_cMaterialVertexFormat = _T("MaterialVertexFormat");
	ms_cLightFunKey= _T("LightFunKey");
	ms_cMaterialLightKey = _T("MaterialLightKey");
	ms_cMaterialVertex =_T("MaterialVertex");
	ms_cNormalDepthVertex = _T("NormalDepthVertex");
	ms_cCubShadowVertex = _T("CubShadowVertex");
	ms_cVolumeShadowVertex = _T("VolumeShadowVertex");
	ms_cVolumeVertexFormat = _T("VolumeVertexFormat");
	ms_cShadowVertex = _T("ShadowVertex");
	ms_cDualParaboloidShadowVertex = _T("DualParaboloidShadowVertex");
	ms_cLightFunVertex = _T("LightFunVertex");
	ms_cPostEffectVertex = _T("PostEffectVertex");

	ms_cPrezVertex = _T("PrezVertex");
	ms_cPrezPiexl = _T("PrezPiexl");
	ms_cPostGray =_T("PostGray");
	ms_cGammaCorrect = _T("GammaCorrect");
	ms_cPostScreenQuad = _T("PostScreenQuad");
	ms_cPostVolumeShadowMap = _T("PostVolumeShadowMap");
	ms_cPostInputTexture = _T("PostInputTexture");
	ms_cPostInv_Width = _T("PostInv_Width");
	ms_cPostInv_Height = _T("PostInv_Height");
	ms_cBlinnPhong = _T("BlinnPhone");
	ms_cOrenNayarLookUpTable = _T("OrenNayarLookUpTable");
	ms_cStraussLookUpTable = _T("StraussLookUpTable");
	ms_cCookTorranceSpecluarType = _T("CookTorranceSpecluarType");
	ms_cIsotropicWarLookUpTable = _T("IsotropicWarLookUpTable");
	ms_cPassID = _T("PassID");
	ms_cMainScene = _T("Main");
	ms_cLighted = _T("bLighted");
	ms_cPointLightShadowNum = _T("PointLightShadowNum");
	ms_cPointLightVolumeShadowNum = _T("PointLightVolumeShadowNum");
	ms_cPointLightParaboloidShadowNum = _T("PointLightParaboloidShadowNum");
	ms_cDirectionLightShadowNum = _T("DirectionLightShadowNum");
	ms_cDirectionLightVolumeShadowNum = _T("DirectionLightVolumeShadowNum");
	ms_cDirectionLightCSMShadowNum = _T("DirectionLightCSMShadowNum");
	ms_cSpotLightShadowNum = _T("SpotLightShadowNum");

	ms_cCubShadowFov = _T("CubShadowFov");


	ms_cProjectShadowPreVertex = _T("ProjectShadowPreVertex");
	ms_cProjectShadowPrePixel = _T("ProjectShadowPrePixel");

	ms_cProjectShadowVertex = _T("ProjectShadowVertex");
	ms_cProjectShadowPixel = _T("ProjectShadowPixel");

	ms_cFontColor = _T("FontColor");
	return 1;
}
bool VSUsedName::TerminalDefaultState()
{
	return 1;
}
void VSUsedName::Archive(VSStream & Stream)
{
	if (Stream.GetStreamFlag() == VSStream::AT_LOAD)
	{
		VSString String;
		Stream.ReadString(String);
		if (String == VSString::ms_StringNULL)
		{
			m_pName = NULL;
		}
		else
		{
			m_pName = VSResourceManager::CreateName(String);
		}
	}
	else if (Stream.GetStreamFlag() == VSStream::AT_SAVE)
	{
		if (m_pName)
		{
			Stream.WriteString(m_pName->GetString());
		}
		else
		{
			Stream.WriteString(VSString::ms_StringNULL);
		}
	}
	else if (Stream.GetStreamFlag() == VSStream::AT_SIZE)
	{
		if (m_pName)
		{

			Stream.AddBufferSize(VSStream::GetStrDistUse(m_pName->GetString()));
		}
		else
		{

			Stream.AddBufferSize(VSStream::GetStrDistUse(VSString::ms_StringNULL));
		}
	}
	
}
void VSUsedName::CopyFrom(VSCustomArchiveObject * pObject,VSMap<VSObject *,VSObject*>& CloneMap)
{
	VSUsedName * pUsedName = (VSUsedName *)pObject;
	*this = *pUsedName;
}
namespace VSEngine2
{
bool operator ==(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 1;
	}
	else if (!Name1.m_pName || !Name2.m_pName)
	{
		return 0;
	}
	return Name1.m_pName->GetID() == Name2.m_pName->GetID();
}


bool operator !=(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (!Name1.m_pName || !Name2.m_pName)
	{
		return 1;
	}
	return Name1.m_pName->GetID() != Name2.m_pName->GetID();
}

bool operator ==(const VSUsedName &Name,const VSString & String)
{
	if (!Name.m_pName)
	{
		return 0;
	}
	return Name.m_pName->GetString() == String;
}


bool operator !=(const VSUsedName &Name,const VSString & String)
{
	if (!Name.m_pName)
	{
		return 1;
	}
	return Name.m_pName->GetString() != String;
}

bool operator ==(const VSUsedName &Name,const TCHAR * pChar)
{
	if (!Name.m_pName && !pChar)
	{
		return 1;
	}
	else if (!Name.m_pName || !pChar)
	{
		return 0;
	}
	return Name.m_pName->GetString() == pChar;
}


bool operator !=(const VSUsedName &Name,const TCHAR * pChar)
{
	if (!Name.m_pName && !pChar)
	{
		return 0;
	}
	else if (!Name.m_pName || !pChar)
	{
		return 1;
	}
	return Name.m_pName->GetString() != pChar;
}

bool operator >(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (Name1.m_pName && !Name2.m_pName)
	{
		return 1;
	}
	else if (!Name1.m_pName && Name2.m_pName)
	{
		return 0;
	}
	return Name1.m_pName->GetID() > Name2.m_pName->GetID();
}

bool operator <(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (!Name1.m_pName && Name2.m_pName)
	{
		return 1;
	}
	return Name1.m_pName->GetID() < Name2.m_pName->GetID();
}
}