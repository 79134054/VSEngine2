/*----------------------------------------------------------------*/
FORCEINLINE VSVector3 VSSurface3::GetN(VSREAL U,VSREAL V)
{
	VSVector3 N;
	N.Cross(GetUPartialDerivative(U,V),GetVPartialDerivative(U,V));
	return N;

}
/*----------------------------------------------------------------*/