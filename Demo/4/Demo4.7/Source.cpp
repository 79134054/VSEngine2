//这个示例演示了VSMatch的使用
#include <VSMemManager.h>
#include <VSMatch.h>
#include <iostream>
using namespace VSEngine2;
void main()
{
	VSMatch Match;
	if (!Match.Open(_T("Test.txt")))
		return;
		

	if (!Match.Getline())
	{
		return;
	} // end if

	if (Match.PatternMatch("['TexturePath'][s>0]"))
	{
		std::cout << Match.pstrings[0] << std::endl;
	} // end if

	if (!Match.Getline())
	{
		return;
	} // end if

	if (Match.PatternMatch("['MaxHealth'][i]"))
	{
		std::cout << Match.pints[0] << std::endl;
	} // end if

	if (!Match.Getline())
	{
		return;
	} // end if

	if (Match.PatternMatch("['Direction'][f][f][f]"))
	{
		std::cout << Match.pfloats[0] << " " << Match.pfloats[1] << " " << Match.pfloats[2] << std::endl;
	} // end if

	getchar();
}