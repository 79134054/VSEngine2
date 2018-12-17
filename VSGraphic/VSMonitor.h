
#ifndef VSMONITOR_H
#define VSMONITOR_H
#include "VSGraphic.h"
#include "VSSynchronize.h"
#include "VSThread.h"
#include "VSString.h"
#include "VSArray.h"
#include "VSResource.h"
#include "VSSafeQueue.h"
#ifdef WINDOWS
namespace VSEngine2
{
	struct FileOperation
	{
		enum Type
		{
			Added,
			Removed, 
			Modified
		};
	};
	struct FileChangedArgs
	{
		FileOperation::Type	Operation;
		VSString			FilePath;
		bool operator==(const FileChangedArgs& rhs) const;
	};
	FORCEINLINE bool FileChangedArgs::operator==(const FileChangedArgs& rhs) const
	{
		return (this->Operation == rhs.Operation) &&
			(this->FilePath == rhs.FilePath);
	}
	class VSGRAPHIC_API VSResourceChangedHandler
	{
	public:
		VSResourceChangedHandler()
		{

		}
		virtual ~VSResourceChangedHandler() {}
		// 检测文件类型是否可以处理
		bool CheckFileType(const VSString & path)
		{
			return path.GetSubStringIndex(VSResource::GetFileSuffix(GetResourceType()), 1) != -1;
		}
		// 处理文件更新事件
		virtual void OnFileChanged(const FileChangedArgs& args) = 0;

		virtual unsigned int GetResourceType() const = 0;
	};
	class VSGRAPHIC_API VSAnimChangedHandler : public VSResourceChangedHandler
	{
			
	public:
		VSAnimChangedHandler()
		{

		}
		virtual ~VSAnimChangedHandler()
		{

		}
		virtual unsigned int GetResourceType() const
		{
			return VSResource::RT_ACTION;
		}
		/// 处理文件更新事件
		virtual void OnFileChanged(const FileChangedArgs& args)
		{

		}
	};
	class VSMonitor; 
	class VSGRAPHIC_API VSFileMonitor : public VSThread
	{
	public:
		VSFileMonitor(VSMonitor * pOwner);
		virtual ~VSFileMonitor();

		// 设置监视目录
		void SetDirectory(const VSString& s);
		// 取得监视目录
		const VSString & GetDirectory() const;

		// 设置是否使用相对路径
		void SetUseRelativePath(bool b);
		// 是否使用相对路径
		bool IsUseRelativePath() const;

	private:
		virtual void Run();
		virtual const TCHAR* GetThreadName();

		void NotifyHandler();
	private:
		static const size_t BufferSize = 4096;
		VSString m_directory;
		bool m_isUseRelativePath;
		unsigned char* m_pReadBuffer;
		VSMonitor * m_pOwner;
	};

	//------------------------------------------------------------------------------
	FORCEINLINE const VSString& VSFileMonitor::GetDirectory() const
	{
		return m_directory;
	}

	//------------------------------------------------------------------------------
	FORCEINLINE void VSFileMonitor::SetDirectory(const VSString& s)
	{

		m_directory = s;
	}

	//------------------------------------------------------------------------------
	FORCEINLINE void VSFileMonitor::SetUseRelativePath(bool b)
	{
		m_isUseRelativePath = b;
	}

	//------------------------------------------------------------------------------
	FORCEINLINE bool VSFileMonitor::IsUseRelativePath() const
	{
		return m_isUseRelativePath;
	}
	class VSGRAPHIC_API VSMonitor 
	{
	public:
		VSMonitor()
		{

		}
		virtual ~VSMonitor()
		{

		}
	protected:
		friend class VSFileMonitor;
		virtual void OnFileChanged(const FileChangedArgs& args) = 0;
	};
	class VSGRAPHIC_API VSResourceMonitor : public VSMonitor
	{
	public:
		VSResourceMonitor();
		virtual ~VSResourceMonitor();

		/// 开始监视
		bool Open();
		/// 更新
		void Update(double dAppTime);
		/// 结束监视
		void Close();

		/// 设置监视目录
		void SetMonitorDirectory(const VSString& path);

		/// 注册文件类型处理接口
		void RegisterHandler(VSResourceChangedHandler* handler);
		/// 删除对应处理接口
		void UnregisterHandler(VSResourceChangedHandler* handler);
		static VSResourceMonitor * ms_pResourceMonitor;
	protected:
		void OnFileChanged(const FileChangedArgs& args);

	private:
		VSFileMonitor* m_fileMonitor;
		VSSafeQueue<FileChangedArgs> m_changedFiles;
		VSArray<VSResourceChangedHandler*> m_handlers;///< in main thread
	};
};
#endif
#endif
