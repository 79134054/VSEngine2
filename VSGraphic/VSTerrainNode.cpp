#include "VSTerrainNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
#include "VSStream.h"
IMPLEMENT_RTTI_NoCreateFun(VSTerrainNode,VSMeshNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSTerrainNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSTerrainNode,VSMeshNode)
REGISTER_PROPERTY(m_uiTesselationLevel,TesselationLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNumX,NumX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiNumZ,NumZ,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fHeightScale,HeightScale,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pHeight,m_uiTotleNum,HeightData);
END_ADD_PROPERTY

VSTerrainNode::VSTerrainNode()
{
	m_uiTesselationLevel = MIN_TESSELATION_LEVEL;
	m_uiNumX = MIN_NUM;
	m_uiNumZ = MIN_NUM;

	m_pHeight = NULL;
	m_fHeightScale = 1.0f;
	m_uiTotleNum = 0;
}

VSTerrainNode::~VSTerrainNode()
{
	VSMAC_DELETEA(m_pHeight);
}

void VSTerrainNode::SetTesselationLevel(unsigned int uiTesselationLevel)
{
	m_uiTesselationLevel = uiTesselationLevel;
	if(m_uiTesselationLevel > m_uiNumX)
	{
		m_uiTesselationLevel = m_uiNumX;
	}

	if(m_uiTesselationLevel > m_uiNumZ)
	{
		m_uiTesselationLevel = m_uiNumZ;
	}

	if (m_uiTesselationLevel > MAX_TESSELATION_LEVEL)
	{
		m_uiTesselationLevel = MAX_TESSELATION_LEVEL;
	}
	if (m_uiTesselationLevel < MIN_TESSELATION_LEVEL)
	{
		m_uiTesselationLevel = MIN_TESSELATION_LEVEL;
	}
}
void VSTerrainNode::SetNum(unsigned int uiNumX,unsigned int uiNumZ)
{
	m_uiNumX = uiNumX;
	m_uiNumZ = uiNumZ;
	if (m_uiNumX > MAX_NUM)
	{
		m_uiNumX = MAX_NUM;
	}
	if(m_uiNumX < MIN_NUM)
	{
		m_uiNumX = MIN_NUM;
	}
	
	if (m_uiNumZ > MAX_NUM)
	{
		m_uiNumZ = MAX_NUM;
	}
	if(m_uiNumZ < MIN_NUM)
	{
		m_uiNumZ = MIN_NUM;
	}
	if (m_uiTesselationLevel > m_uiNumX || m_uiTesselationLevel > m_uiNumZ)
	{
		SetTesselationLevel(m_uiTesselationLevel);
	}
	m_uiTotleNum = ((1 << m_uiNumX) + 1) * ((1 << m_uiNumZ) + 1);
}

bool VSTerrainNode::CreateTerrain(unsigned int uiNumX,unsigned int uiNumZ,unsigned int uiTesselationLevel)
{
	
	SetNum(uiNumX,uiNumZ);
	SetTesselationLevel(uiTesselationLevel);
	VSMAC_DELETEA(m_pHeight);
	m_pHeight = VS_NEW unsigned char[m_uiTotleNum];
	if (!m_pHeight)
	{
		return 0;
	}
	VSMemset(m_pHeight,0,m_uiTotleNum * sizeof(unsigned char));
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}
bool VSTerrainNode::CreateTarrainFromHeightMap(const TCHAR *pFileName,unsigned int uiTesselationLevel,VSREAL fHeightScale)
{
	if (!pFileName)
	{
		return 0;
	}
	
	VSFile *pFile = NULL;
	pFile = VS_NEW VSFile();
	if (!pFile)
	{
		return 0;
	}

	unsigned int uiHeightSize = 0;
	VSMAC_DELETEA(m_pHeight);

	VSString TerrainPath = VSResourceManager::ms_TerrainPath + pFileName;
	if(!pFile->Open(TerrainPath.GetBuffer(),VSFile::OM_RB))
	{
		VSMAC_DELETE(pFile);
		return 0;
	}
	
	pFile->Read(&uiHeightSize, 1, sizeof(unsigned int));
	m_pHeight= VS_NEW unsigned char [uiHeightSize * uiHeightSize];

	if (!m_pHeight)
	{
		return 0;
	}

	pFile->Read(m_pHeight, 1, uiHeightSize * uiHeightSize);

	VSMAC_DELETE(pFile);

	unsigned int uiNum = 0;
	uiHeightSize = uiHeightSize - 1;
	_asm bsr eax, uiHeightSize
	_asm mov uiNum, eax

	SetNum(uiNum,uiNum);
	SetTesselationLevel(uiTesselationLevel);
	m_fHeightScale = fHeightScale;
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}

unsigned char VSTerrainNode::GetRawHeight(unsigned int uiX,unsigned int uiZ)
{
	unsigned int uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ];
	}
	return 0;
}
VSREAL VSTerrainNode::GetHeight(unsigned int uiX,unsigned int uiZ)
{
	unsigned int uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ] * m_fHeightScale;
	}
	return 0.0f;

}
