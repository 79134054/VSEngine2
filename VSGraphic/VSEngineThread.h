#ifndef VSENGINETHREAD_H
#define VSENGINETHREAD_H
#include "VSObject.h"
namespace VSEngine2
{
	class VSRenderCommand
	{
	public:
		VSRenderCommand(){}
		virtual ~VSRenderCommand(){}
		virtual unsigned int Execute() = 0;
		virtual const TCHAR * DescribeCommand() = 0;
	protected:
	};


	class VSGRAPHIC_API VSConstantBuffer : public VSMemObject
	{
	public:
		enum
		{
			Constant_BUFFER_SIZE = 6 * 1024 * 1024
		};
		//VSUserConstant Type
		VSConstantBuffer();
		~VSConstantBuffer();
		unsigned char * Assign(unsigned int uiSize);
		unsigned char * Assign(unsigned int VTType,unsigned int uiRegisterNum);
		void Clear();
		friend class VSRenderThreadBuffer;
	protected:
		VSCriticalSection m_Lock;
		VSArray<unsigned char> m_Buffer;
		unsigned int m_uiCurBufferP;
	};
	#define DECLARE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
	class TypeName : public VSRenderCommand \
	{ \
	public: \
		TypeName() \
		{}\
		~TypeName()\
		{ \
		} \
		virtual unsigned int Execute() \
		{ \
			Code; \
			return sizeof(*this); \
		} \
		virtual const TCHAR* DescribeCommand() \
		{ \
			return _T( #TypeName ); \
		} \
	};

	#define DECLARE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	class TypeName : public VSRenderCommand \
	{ \
	public: \
		typedef ParamType1 _ParamType1; \
		TypeName() \
		{}\
		TypeName(const _ParamType1& In##ParamName1): \
			ParamName1(In##ParamName1) \
		{} \
		~TypeName()\
		{ \
		} \
		virtual unsigned int Execute() \
		{ \
			Code; \
			return sizeof(*this); \
		} \
		virtual const TCHAR* DescribeCommand() \
		{ \
			return _T( #TypeName ); \
		} \
	private: \
		ParamType1 ParamName1; \
	}; 
	#define DECLARE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	class TypeName : public VSRenderCommand \
	{ \
	public: \
		typedef ParamType1 _ParamType1; \
		typedef ParamType2 _ParamType2; \
		TypeName() \
		{}\
		~TypeName()\
		{ \
		} \
		TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2): \
			ParamName1(In##ParamName1), \
			ParamName2(In##ParamName2) \
		{} \
		virtual unsigned int Execute() \
		{ \
			Code; \
			return sizeof(*this); \
		} \
		virtual const TCHAR* DescribeCommand() \
		{ \
			return _T( #TypeName ); \
		} \
	private: \
		ParamType1 ParamName1; \
		ParamType2 ParamName2; \
	};
	#define DECLARE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	class TypeName : public VSRenderCommand \
	{ \
	public: \
		typedef ParamType1 _ParamType1; \
		typedef ParamType2 _ParamType2; \
		typedef ParamType3 _ParamType3; \
		TypeName() \
		{}\
		TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3): \
			ParamName1(In##ParamName1), \
			ParamName2(In##ParamName2), \
			ParamName3(In##ParamName3) \
		{} \
		~TypeName()\
		{ \
		} \
		virtual unsigned int Execute() \
		{ \
			Code; \
			return sizeof(*this); \
		} \
		virtual const TCHAR* DescribeCommand() \
		{ \
			return _T( #TypeName ); \
		} \
	private: \
		ParamType1 ParamName1; \
		ParamType2 ParamName2; \
		ParamType3 ParamName3; \
	};
}
#endif