FORCEINLINE const VSMatrix3X3W & VSTransform::GetCombine()
{

	Combine();

	return m_mCombine;
}
FORCEINLINE const VSMatrix3X3W & VSTransform::GetCombineInverse()
{

	Combine();

	if (!m_bIsHaveInverse)
	{
		m_bIsHaveInverse = 1;
		m_mCombineInverse.InverseOf(m_mCombine);
	}
	return m_mCombineInverse;

}
FORCEINLINE const VSVector3&  VSTransform::GetScale()const
{
	return m_fScale;

}
FORCEINLINE const VSVector3 & VSTransform::GetTranslate()const
{
	return m_vTranslate;

}
FORCEINLINE const VSMatrix3X3 & VSTransform::GetRotate()const
{
	return m_mRotate;

}
FORCEINLINE void VSTransform::SetScale(const VSVector3& fScale)
{

	m_fScale = fScale;

	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
FORCEINLINE void VSTransform::SetTranslate(const VSVector3& Translate)
{
	m_vTranslate = Translate;
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
FORCEINLINE void VSTransform::SetRotate(const VSMatrix3X3 & Rotate)
{
	m_mRotate = Rotate;
	
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;

}
void VSTransform::GetDir(VSVector3 &Dir,VSVector3 &Up,VSVector3 & Right)const
{
	Right.x = m_mRotate._00;
	Right.y = m_mRotate._01;
	Right.z = m_mRotate._02;

	Up.x = m_mRotate._10;
	Up.y = m_mRotate._11;
	Up.z = m_mRotate._12;

	Dir.x = m_mRotate._20;
	Dir.y = m_mRotate._21;
	Dir.z = m_mRotate._22;
}