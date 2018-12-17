/********************************************************************************/
FORCEINLINE const VSString &VSRtti::GetName()const
{
	return m_cRttiName;

}//end GetName
/********************************************************************************/
FORCEINLINE bool VSRtti::IsSameType(const VSRtti &Tpye) const
{
	return  (&Tpye == this);
}//end IsSameType
/********************************************************************************/
FORCEINLINE bool VSRtti::IsDerived(const VSRtti &Tpye) const
{
	const VSRtti * pTemp = this;
	while(!pTemp->IsSameType(Tpye))
	{
		if(pTemp->m_pBase)
		{
			pTemp = pTemp->m_pBase;
		
		}
		else
		{
			return 0;
		}
	
	}
	return 1;
}//end IsDerived
/********************************************************************************/