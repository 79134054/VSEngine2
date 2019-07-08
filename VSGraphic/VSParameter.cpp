#include "VSParameter.h"
using namespace VSEngine2;
unsigned int VSGlobalParameter::ms_GpuSkinBoneNum = 50;
bool VSGlobalParameter::ms_bDrawSkelecton = true;
bool VSGlobalParameter::ms_bIsCacheName = false;
bool VSGlobalParameter::ms_bIsCacheShader = true;
bool VSGlobalParameter::ms_bIsCacheTexture = false;
bool VSGlobalParameter::ms_bIsCacheVertexFormat = false;
bool VSGlobalParameter::ms_bIsCacheFont = false;
bool VSGlobalParameter::ms_bIsCacheSkelectionMesh = false;
bool VSGlobalParameter::ms_bIsCacheStaticMesh = false;
bool VSGlobalParameter::ms_bIsCacheAnimSet = false;
bool VSGlobalParameter::ms_bIsCacheMaterial = false;
VSGlobalParameter::VSGlobalParameter()
{

}
VSGlobalParameter::~VSGlobalParameter()
{


}