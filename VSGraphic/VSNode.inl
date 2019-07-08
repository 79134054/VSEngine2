FORCEINLINE unsigned int VSNode::GetNodeNum()const
{
	return m_pChild.GetNum();
}
FORCEINLINE VSArray<VSSpatialPtr> * VSNode::GetChildList()
{
	return &m_pChild;
}
