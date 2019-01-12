#ifndef VSMEMMANAGER_H
#define VSMEMMANAGER_H
#include "VSSystem.h"
#include "VSSynchronize.h"
#include <Windows.h>
#include <new.h>
#define VS_NEW new
#define VS_DELETE delete
//#define USE_STL_TYPE_TRAIT
#ifdef USE_STL_TYPE_TRAIT
	#include <type_traits>
#endif // USE_STL_TYPE_TRAIT

namespace VSEngine2
{

#ifdef USE_STL_TYPE_TRAIT

	#define HAS_TRIVIAL_CONSTRUCTOR(T) std::has_trivial_constructor<T>::value
	#define HAS_TRIVIAL_DESTRUCTOR(T) std::has_trivial_destructor<T>::value
	#define HAS_TRIVIAL_ASSIGN(T) std::has_trivial_assign<T>::value
	#define HAS_TRIVIAL_COPY(T) std::has_trivial_copy<T>::value
	#define IS_POD(T) std::is_pod<T>::value
	#define IS_ENUM(T) std::is_enum<T>::value
	#define IS_EMPTY(T) std::is_empty<T>::value
	

	/**
	* TIsFloatType
	*/
	template<typename T> struct TIsFloatType { enum { Value = std::is_floating_point<T>::value }; };


	/**
	* TIsIntegralType
	*/
	template<typename T> struct TIsIntegralType { enum { Value = std::is_integral<T>::value }; };

	
	/**
	* TIsArithmeticType
	*/
	template<typename T> struct TIsArithmeticType 
	{ 
		enum { Value = std::is_arithmetic<T>::value };
	};

	/**
	* TIsPointerType
	* @todo - exclude member pointers
	*/
	template<typename T> struct TIsPointerType	{ enum { Value = std::is_pointer<T>::value }; };


	/**
	* TIsVoidType
	*/
	template<typename T> struct TIsVoidType { enum { Value = std::is_void<T>::value }; };


	/**
	* TIsPODType
	*/
	template<typename T> struct TIsPODType 
	{ 
		enum { Value = IS_POD(T)}; 
	};

	/**
	* TIsFundamentalType
	*/
	template<typename T> 
	struct TIsFundamentalType 
	{ 
		enum { Value = std::is_fundamental<T>::Value };
	};

	template<typename T> struct ValueBase
	{
		enum { NeedsConstructor = !HAS_TRIVIAL_CONSTRUCTOR(T) && !TIsPODType<T>::Value };
		enum { NeedsDestructor = !HAS_TRIVIAL_DESTRUCTOR(T) && !TIsPODType<T>::Value };
	};
#else
	#if _MSC_VER >= 1400
	#define HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
	#define HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
	#define HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
	#define HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
	#define IS_POD(T) __is_pod(T)
	#define IS_ENUM(T) __is_enum(T)
	#define IS_EMPTY(T) __is_empty(T)
	#else
	#define HAS_TRIVIAL_CONSTRUCTOR(T) false
	#define HAS_TRIVIAL_DESTRUCTOR(T) false
	#define HAS_TRIVIAL_ASSIGN(T) false
	#define HAS_TRIVIAL_COPY(T) false
	#define IS_POD(T) false
	#define IS_ENUM(T) false
	#define IS_EMPTY(T) false
	#endif


	/*-----------------------------------------------------------------------------
	Type traits similar to TR1 (uses intrinsics supported by VC8)
	Should be updated/revisited/discarded when compiler support for tr1 catches up.
	-----------------------------------------------------------------------------*/

	/**
	* TIsFloatType
	*/
	template<typename T> struct TIsFloatType { enum { Value = false }; };

	template<> struct TIsFloatType<float> { enum { Value = true }; };
	template<> struct TIsFloatType<double> { enum { Value = true }; };
	template<> struct TIsFloatType<long double> { enum { Value = true }; };

	/**
	* TIsIntegralType
	*/
	template<typename T> struct TIsIntegralType { enum { Value = false }; };

	template<> struct TIsIntegralType<unsigned char> { enum { Value = true }; };
	template<> struct TIsIntegralType<unsigned short> { enum { Value = true }; };
	template<> struct TIsIntegralType<unsigned int> { enum { Value = true }; };
	template<> struct TIsIntegralType<unsigned long> { enum { Value = true }; };

	template<> struct TIsIntegralType<signed char> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed short> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed int> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed long> { enum { Value = true }; };

	template<> struct TIsIntegralType<bool> { enum { Value = true }; };
	template<> struct TIsIntegralType<char> { enum { Value = true }; };

	// compilers we support define wchar_t as a native type
#if !_MSC_VER || defined(_NATIVE_WCHAR_T_DEFINED)
	template<> struct TIsIntegralType<wchar_t> { enum { Value = true }; };
#endif

	// C99, but all compilers we use support it
	template<> struct TIsIntegralType<unsigned long long> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed long long> { enum { Value = true }; };
	/**
	* TIsArithmeticType
	*/
	template<typename T> struct TIsArithmeticType 
	{ 
		enum { Value = TIsIntegralType<T>::Value || TIsFloatType<T>::Value } ;
	};

	/**
	* TIsPointerType
	* @todo - exclude member pointers
	*/
	template<typename T> struct TIsPointerType						{ enum { Value = false }; };
	template<typename T> struct TIsPointerType<T*>					{ enum { Value = true }; };
	template<typename T> struct TIsPointerType<const T*>			{ enum { Value = true }; };
	template<typename T> struct TIsPointerType<const T* const>		{ enum { Value = true }; };
	template<typename T> struct TIsPointerType<T* volatile>			{ enum { Value = true }; };
	template<typename T> struct TIsPointerType<T* const volatile>	{ enum { Value = true }; };

	/**
	* TIsVoidType
	*/
	template<typename T> struct TIsVoidType { enum { Value = false }; };
	template<> struct TIsVoidType<void> { enum { Value = true }; };
	template<> struct TIsVoidType<void const> { enum { Value = true }; };
	template<> struct TIsVoidType<void volatile> { enum { Value = true }; };
	template<> struct TIsVoidType<void const volatile> { enum { Value = true }; };

	/**
	* TIsPODType
	* @todo - POD array and member pointer detection
	*/
	template<typename T> struct TIsPODType 
	{ 
		enum { Value = IS_POD(T) || IS_ENUM(T) || TIsArithmeticType<T>::Value || TIsPointerType<T>::Value }; 
	};

	/**
	* TIsFundamentalType
	*/
	template<typename T> 
	struct TIsFundamentalType 
	{ 
		enum { Value = TIsArithmeticType<T>::Value || TIsVoidType<T>::Value };
	};


	template<typename T> struct ValueBase
	{
		// WRH - 2007/11/28 - the compilers we care about do not produce equivalently efficient code when manually
		// calling the constructors of trivial classes. In array cases, we can call a single memcpy
		// to initialize all the members, but the compiler will call memcpy for each element individually,
		// which is slower the more elements you have. 
		enum { NeedsConstructor = !HAS_TRIVIAL_CONSTRUCTOR(T) && !TIsPODType<T>::Value };
		// WRH - 2007/11/28 - the compilers we care about correctly elide the destructor code on trivial classes
		// (effectively compiling down to nothing), so it is not strictly necessary that we have NeedsDestructor. 
		// It doesn't hurt, though, and retains for us the ability to skip destructors on classes without trivial ones
		// if we should choose.
		enum { NeedsDestructor = !HAS_TRIVIAL_DESTRUCTOR(T) && !TIsPODType<T>::Value };
	};
#endif
	template< class T > FORCEINLINE T Align( const T Ptr, unsigned int Alignment )
	{
		return (T)(((unsigned int)Ptr + Alignment - 1) & ~(Alignment-1));

	}
	template< class T > FORCEINLINE T Align1( const T Ptr, unsigned int Alignment )
	{
		return (T)((unsigned int)Ptr + Alignment - (Ptr & (Alignment-1)));
	}
	class VSSYSTEM_API VSMemManager
	{
	public:
		VSMemManager();
		virtual ~VSMemManager() = 0;
		virtual void* Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray) = 0;
		virtual void Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray) = 0;
		
	};
	class VSSYSTEM_API VSCMem : public VSMemManager
	{
	public:
		VSCMem();
		~VSCMem();
		void* Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray);
		void Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray);
	};
	class VSSYSTEM_API VSDebugMem : public VSMemManager
	{
	public:
		VSDebugMem();
		~VSDebugMem();

		virtual void* Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray);
		virtual void Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray);

	private:
		enum
		{
			BEGIN_MASK = 0xDEADC0DE,
			END_MASK = 0xDEADC0DE,
			RECORD_NUM = 32, //必须大于2
			CALLSTACK_NUM = 32
		};

		static VSCriticalSection ms_MemLock;
		class Block
		{
		public:
			Block()
			{
				for (unsigned int i = 0; i < CALLSTACK_NUM; i++)
				{
					pAddr[i] = NULL;
				}
				m_pPrev = NULL;
				m_pNext = NULL;
			}
			void * pAddr[CALLSTACK_NUM];	//申请内存时候的调用堆栈信息
			unsigned int m_uiStackInfoNum;	//堆栈层数
			unsigned int m_uiSize;			//申请空间的大小
			bool m_bIsArray;				//是否是数组
			bool m_bAlignment;				//是否字节对齐
			Block * m_pPrev;				//前一个节点
			Block * m_pNext;				//后一个节点
		};
		unsigned int m_uiNumNewCalls;
		unsigned int m_uiNumDeleteCalls;
		Block* m_pHead;
		Block* m_pTail;
		unsigned int m_uiNumBlocks;
		unsigned int m_uiNumBytes;
		unsigned int m_uiMaxNumBytes;
		unsigned int m_uiMaxNumBlocks;
		unsigned int m_uiSizeRecord[RECORD_NUM];
		void InsertBlock (Block* pBlock);
		void RemoveBlock (Block* pBlock);
		bool GetFileAndLine(const void *pAddress, TCHAR szFile[MAX_PATH], int &line);
		bool InitDbgHelpLib();
		void FreeDbgHelpLib();
		void FreeLeakMem();
		void PrintInfo();
	};

	//this code copy from U3 FMallocWindows.h , if you want to kown how it to work,please
	// visit my blog 
	class VSSYSTEM_API VSMemWin32 : public VSMemManager
	{
	public:
		VSMemWin32();
		~VSMemWin32();
		virtual void* Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray);
		virtual void Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray);
	private:
		static VSCriticalSection ms_MemLock;
		// Counts.
		enum {POOL_COUNT = 42     };
		enum {POOL_MAX   = 32768+1};

		// Forward declares.
		struct FFreeMem;
		struct FPoolTable;

		// Memory pool info. 32 bytes.
		struct FPoolInfo
		{
			DWORD	    Bytes;		// Bytes allocated for pool.
			DWORD		OsBytes;	// Bytes aligned to page size.
			DWORD       Taken;      // Number of allocated elements in this pool, when counts down to zero can free the entire pool.
			BYTE*       Mem;		// Memory base.
			FPoolTable* Table;		// Index of pool.
			FFreeMem*   FirstMem;   // Pointer to first free memory in this pool.
			FPoolInfo*	Next;
			FPoolInfo**	PrevLink;

			void Link( FPoolInfo*& Before )
			{
				if( Before )
				{
					Before->PrevLink = &Next;
				}
				Next     = Before;
				PrevLink = &Before;
				Before   = this;
			}
			void Unlink()
			{
				if( Next )
				{
					Next->PrevLink = PrevLink;
				}
				*PrevLink = Next;
			}
		};

		// Information about a piece of free memory. 8 bytes.
		struct FFreeMem
		{
			FFreeMem*	Next;		// Next or MemLastPool[], always in order by pool.
			DWORD		Blocks;		// Number of consecutive free blocks here, at least 1.
			FPoolInfo* GetPool();
		};

		// Pool table.
		struct FPoolTable
		{
			FPoolInfo* FirstPool;
			FPoolInfo* ExhaustedPool;
			DWORD      BlockSize;
		};


		FPoolTable  PoolTable[POOL_COUNT], OsTable;
		FPoolInfo*	PoolIndirect[32];
		FPoolTable* MemSizeToPoolTable[POOL_MAX];
		INT			PageSize;

		FPoolInfo* CreateIndirect()
		{
			FPoolInfo* Indirect = (FPoolInfo*)VirtualAlloc( NULL, 2048*sizeof(FPoolInfo), MEM_COMMIT, PAGE_READWRITE );
			if( !Indirect )
			{
				return NULL;
			}
			return Indirect;
		}
	};
	// this is Stack Mem ,it will be clear every tick ,now No considering thread safe .
	// this code is modified by u3
	class VSSYSTEM_API VSStackMem : public VSMemManager
	{
	public:
		VSStackMem(unsigned int uiDefaultChunkSize = 65536);
		~VSStackMem();
		void* Allocate (unsigned int uiSize,unsigned int uiAlignment,bool bIsArray);
		void Deallocate (char* pcAddr, unsigned int uiAlignment,bool bIsArray)
		{
			return;
		}
		template<class T>
		friend class VSStackMemAlloc;
		
		//每帧结束或者开始的时候调用
		void Clear();
	private:

		// Types.
		struct FTaggedMemory
		{
			FTaggedMemory* Next;
			INT DataSize;
			BYTE Data[1];
		};

		// Variables.
		BYTE*			Top;				// Top of current chunk (Top<=End).
		BYTE*			End;				// End of current chunk.
		unsigned int	DefaultChunkSize;	// Maximum chunk size to allocate.
		FTaggedMemory*	TopChunk;			// Only chunks 0..ActiveChunks-1 are valid.

		/** The memory chunks that have been allocated but are currently unused. */
		FTaggedMemory*	UnusedChunks;

		/** The number of marks on this stack. */
		INT NumMarks;

		/**
		* Allocate a new chunk of memory of at least MinSize size,
		* and return it aligned to Align. Updates the memory stack's
		* Chunks table and ActiveChunks counter.
		*/
		BYTE* AllocateNewChunk( INT MinSize );

		/** Frees the chunks above the specified chunk on the stack. */
		/*移除这个chunk和这个chunk之前的所有chunk*/
		void FreeChunks( FTaggedMemory* NewTopChunk );
	};

	// if the class has memory alloc , you must inherit from VSMemObject , so you can use the MemManger any where 
	class VSSYSTEM_API VSMemObject
	{
	public:
		VSMemObject();
		~VSMemObject();
		static VSStackMem& GetStackMemManager();
		static VSMemManager& GetMemManager();

	};
	typedef VSMemManager& (*VSMemManagerFun)();

	template<class T>
	class VSStackMemAlloc : public VSMemObject
	{
	public:
		// Constructors.
		VSStackMemAlloc(unsigned int uiNum = 0,unsigned int uiAlignment = 0)
		{
			m_uiNum = uiNum;
			Top = GetStackMemManager().Top;
			SavedChunk = GetStackMemManager().TopChunk;		
			// Track the number of outstanding marks on the stack.
			GetStackMemManager().NumMarks++;
			if (m_uiNum > 0)
			{
				m_pPtr = (T *)GetStackMemManager().Allocate(uiNum * sizeof(T),uiAlignment,0);
				VSMAC_ASSERT(m_pPtr);
				if (ValueBase<T>::NeedsConstructor)
				{
					for (unsigned int i = 0 ; i < uiNum ; i++)
					{
						VS_NEW(m_pPtr + i)T();
					}
				}
			}
		}

		/** Destructor. */
		~VSStackMemAlloc()
		{

			if (m_uiNum > 0)
			{
				if (ValueBase<T>::NeedsDestructor)
				{
					for (unsigned int i = 0 ; i < m_uiNum ; i++)
					{
						(m_pPtr + i)->~T();
					}
				}
			}
			// Track the number of outstanding marks on the stack.
			--GetStackMemManager().NumMarks;

			// Unlock any new chunks that were allocated.
			if( SavedChunk != GetStackMemManager().TopChunk )
				GetStackMemManager().FreeChunks( SavedChunk );

			// Restore the memory stack's state.
			GetStackMemManager().Top = Top;

			// Ensure that the mark is only popped once by clearing the top pointer.
			Top = NULL;
		}

		FORCEINLINE T * GetPtr()const
		{
			return m_pPtr;
		}
		FORCEINLINE unsigned int GetNum() const
		{
			return m_uiNum;
		}
	private:
		BYTE* Top;
		VSStackMem::FTaggedMemory* SavedChunk;
		T * m_pPtr;
		unsigned int m_uiNum;
	};
	// 	class VSMemManagerSF
	// 	{
	// 	public:
	// 		class BlockHeader
	// 		{
	// 		public:
	// 			bool m_bUsed;
	// 			unsigned int ms_uiSize;
	// 			BlockHeader * m_pcPrev;
	// 			BlockHeader * m_pcNext;
	// 
	// 		};
	// 		class BlockEnder
	// 		{
	// 		public:
	// 			bool m_bUsed;
	// 			unsigned int ms_uiSize;
	// 		};
	// 		enum
	// 		{
	// 			HEADERSIZE = sizeof(BlockHeader),
	// 			ENDERSIZE = sizeof(BlockEnder)
	// 		};
	// 		static void *SearchByPolicy(unsigned int uiRequestedSize);
	// 		static char * ms_pMemHeapPtr;
	// 		static unsigned int ms_uiSize;
	// 	public:
	// 		static void CreateMemMangerSF(unsigned int uiSize);
	// 		static void DeleteMemMangerSF();
	// 		static void *Allocate(unsigned int uiRequestedSize);
	// 		static void Deallocate(void * pcDelete);
	// 		static BlockHeader * ms_pFreeList;
	// 
	// 	};
	
	
	
}

#define USE_CUSTOM_NEW

#ifdef USE_CUSTOM_NEW
FORCEINLINE void* operator new(size_t uiSize)
{
	return VSEngine2::VSMemObject::GetMemManager().Allocate((unsigned int)uiSize,0,false);
}
FORCEINLINE void* operator new[] (size_t uiSize)
{
	return VSEngine2::VSMemObject::GetMemManager().Allocate((unsigned int)uiSize,0,true);
}

FORCEINLINE void operator delete (void* pvAddr)
{
	return VSEngine2::VSMemObject::GetMemManager().Deallocate((char *)pvAddr,0,false);
}
FORCEINLINE void operator delete[] (void* pvAddr)
{
	return VSEngine2::VSMemObject::GetMemManager().Deallocate((char *)pvAddr,0,true);
}
#endif


#define VSMAC_DELETE(p) if(p){VS_DELETE p; p = 0;}
#define VSMAC_DELETEA(p) if(p){VS_DELETE []p; p = 0;}
#define VSMAC_DELETEAB(p,num) if(p){ for(int i = 0 ; i < num ; i++) VSMAC_DELETEA(p[i]); VSMAC_DELETEA(p);}
// use by inner mac
template<typename T>
FORCEINLINE void VSDelete(T * p)
{
	if (p){ VS_DELETE p; p = 0; }
}
template<typename T>
FORCEINLINE void VSDeleteA(T * p)
{
	if (p){ VS_DELETE[]p; p = 0; }
}
template<typename T,typename N>
FORCEINLINE void VSDeleteAB(T * p,N num)
{
	if (p){ for (int i = 0; i < num; i++) VSMAC_DELETEA(p[i]); VSMAC_DELETEA(p); }
}
#endif