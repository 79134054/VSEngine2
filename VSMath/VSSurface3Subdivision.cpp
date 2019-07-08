#include "VSSurface3Subdivision.h"
#include "VSMemManager.h"
using namespace VSEngine2;
VSSurface3Subdivision::VSSurface3Subdivision()
{
	m_pSurface = NULL;
	m_pPoint = NULL;
	m_uiRow = 0;
	m_uiColumn = 0;
	m_pUV = NULL;
}
/*----------------------------------------------------------------*/
VSSurface3Subdivision::~VSSurface3Subdivision()
{
	VSMAC_DELETEA(m_pPoint);
	VSMAC_DELETEA(m_pUV);
	m_pSurface = NULL;
	m_uiRow = 0;
	m_uiColumn = 0;
}
/*----------------------------------------------------------------*/