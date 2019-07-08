#ifndef VSMATCH_H
#define VSMATCH_H
#include "VSFile.h"
#include "VSContainer.h"
#include "VSArray.h"
#include "VSMath.h"
namespace VSEngine2
{
	/*
	此模式匹配类匹配['name'][s>0][s=5][s<10][f][i]这样以空格分开的字符串 浮点数和整数
	算法实现:由于《3D游戏编程大师技巧》书中实现算法没有给出，而且实现逻辑复杂，代码混乱
	，本人用自动机理论来实现(其实所有和匹配有关的问题基本用自动机和产生式都可以轻松解决)
	具体算法不给出了，如果你掌握了自动机自己可以很轻松的写出来代码的！
	这个模式匹配和以往固定模式匹配不同，模式串不是固定的，所以你要把模式串做为自动机的
	主流程,基本可以分为4个流程，['name']这种固定串匹配,[s>0]这种不定串匹配,[f]浮点匹配
	[i]整数匹配,进入模式串匹配后(也就是模式串匹配后),在判断匹配串是否符合要求！
	*/
	class VSDATESTRUCT_API VSMatch
	{
	private:
		void Clear();
		//模式匹配自动机函数
		//开始状态
		bool Begin();
		// 状态1 
		bool Fun1();
		//状态 2 
		bool Fun2();
		// 固定字符串匹配状态
		bool FunStrMatch();
		// 不定字符串匹配匹配 
		bool FunStr_I_Match();
		// 与模式串字符个数判断匹配
		int FunIntMatch_D();

		// 与匹配串整形匹配
		bool FunIntMatch();
		// 与匹配串浮点串匹配
		bool FunFloMatch();
		VSFile File;
	public:
		enum{BUFFER_SIZE=100,MAX_ARGS=16,MAX_COMMENT=80};
		VSMatch();
		~VSMatch();
		bool Open(TCHAR *filename);
		bool Getline();
		//匹配函数
		bool PatternMatch(TCHAR *_pattern);
		TCHAR buffer[BUFFER_SIZE];			//行缓冲
		TCHAR *pattern;
		int  b_index;
		int	 p_index;
		int  length;						//当前行长度
		int  num_lines;						//处理行数
		//模式返回值
		char  pstrings[MAX_ARGS][BUFFER_SIZE]; // any strings
		int   num_pstrings;
		VSArray<VSREAL> pfloats;               // any floats
		VSArray<int>pints;						// any ints

	}; // end CLASS CPARS
}

#endif