FORCEINLINE void VSPolygon3::Set(const VSVector3 *pPoint,unsigned int PointNum)
{
	if(!pPoint || !PointNum)
		return ;

	m_PointNum = PointNum;
	m_IndexNum = 3 * (m_PointNum - 2);

	VSMAC_DELETEA(m_pPoint);
	VSMAC_DELETEA(m_pIndex);
	m_pPoint = VS_NEW VSVector3[m_PointNum];
	m_pIndex = VS_NEW VSUSHORT_INDEX[m_IndexNum];
	VSMemcpy(m_pPoint,pPoint,sizeof(VSVector3) * PointNum);

	m_Flag = 1;

	unsigned int I0, I1, I2;
	for (unsigned int nLoop = 0 ; nLoop < (m_PointNum - 2); nLoop++) 
	{
		if (nLoop == 0) { I0=0; I1=1; I2=2; }
		else { I1=I2; I2++; }

		m_pIndex[(nLoop*3)   ] = I0;
		m_pIndex[(nLoop*3) +1] = I1;
		m_pIndex[(nLoop*3) +2] = I2;
	}
	
	
	VSVector3 vcEdge0 = m_pPoint[1] - m_pPoint[0];
	VSVector3 vcEdge1;
	//计算所在平面,直到找到不平行的两个线段为止。
	for (unsigned int i = 2; i < PointNum ; i++) 
	{
		vcEdge1 = m_pPoint[i] - m_pPoint[0];

		vcEdge0.Normalize();
		vcEdge1.Normalize();

		//测试是否平行
		VSREAL dot = vcEdge0.Dot(vcEdge1);
		if(ABS(dot) < VSREAL_1)      
			break;
	} // for

	VSVector3 N;
	N.Cross(vcEdge0, vcEdge1);
	VSPlane3::Set(N,m_pPoint[0]);
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSPolygon3::GetPointNum()const
{
	return m_PointNum;
	
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSPolygon3::GetIndexNum()const
{
	return m_IndexNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE const VSVector3 * const VSPolygon3::GetpPoint()const
{

	return m_pPoint;
}
/*----------------------------------------------------------------*/
FORCEINLINE const VSUSHORT_INDEX * const VSPolygon3::GetpIndex()const
{
	return m_pIndex;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int VSPolygon3::GetFlag()const
{
	return m_Flag;
}
/*----------------------------------------------------------------*/