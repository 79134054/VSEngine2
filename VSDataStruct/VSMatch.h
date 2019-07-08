#ifndef VSMATCH_H
#define VSMATCH_H
#include "VSFile.h"
#include "VSContainer.h"
#include "VSArray.h"
#include "VSMath.h"
namespace VSEngine2
{
	/*
	��ģʽƥ����ƥ��['name'][s>0][s=5][s<10][f][i]�����Կո�ֿ����ַ��� ������������
	�㷨ʵ��:���ڡ�3D��Ϸ��̴�ʦ���ɡ�����ʵ���㷨û�и���������ʵ���߼����ӣ��������
	���������Զ���������ʵ��(��ʵ���к�ƥ���йص�����������Զ����Ͳ���ʽ���������ɽ��)
	�����㷨�������ˣ�������������Զ����Լ����Ժ����ɵ�д��������ģ�
	���ģʽƥ��������̶�ģʽƥ�䲻ͬ��ģʽ�����ǹ̶��ģ�������Ҫ��ģʽ����Ϊ�Զ�����
	������,�������Է�Ϊ4�����̣�['name']���̶ֹ���ƥ��,[s>0]���ֲ�����ƥ��,[f]����ƥ��
	[i]����ƥ��,����ģʽ��ƥ���(Ҳ����ģʽ��ƥ���),���ж�ƥ�䴮�Ƿ����Ҫ��
	*/
	class VSDATESTRUCT_API VSMatch
	{
	private:
		void Clear();
		//ģʽƥ���Զ�������
		//��ʼ״̬
		bool Begin();
		// ״̬1 
		bool Fun1();
		//״̬ 2 
		bool Fun2();
		// �̶��ַ���ƥ��״̬
		bool FunStrMatch();
		// �����ַ���ƥ��ƥ�� 
		bool FunStr_I_Match();
		// ��ģʽ���ַ������ж�ƥ��
		int FunIntMatch_D();

		// ��ƥ�䴮����ƥ��
		bool FunIntMatch();
		// ��ƥ�䴮���㴮ƥ��
		bool FunFloMatch();
		VSFile File;
	public:
		enum{BUFFER_SIZE=100,MAX_ARGS=16,MAX_COMMENT=80};
		VSMatch();
		~VSMatch();
		bool Open(TCHAR *filename);
		bool Getline();
		//ƥ�亯��
		bool PatternMatch(TCHAR *_pattern);
		TCHAR buffer[BUFFER_SIZE];			//�л���
		TCHAR *pattern;
		int  b_index;
		int	 p_index;
		int  length;						//��ǰ�г���
		int  num_lines;						//��������
		//ģʽ����ֵ
		char  pstrings[MAX_ARGS][BUFFER_SIZE]; // any strings
		int   num_pstrings;
		VSArray<VSREAL> pfloats;               // any floats
		VSArray<int>pints;						// any ints

	}; // end CLASS CPARS
}

#endif