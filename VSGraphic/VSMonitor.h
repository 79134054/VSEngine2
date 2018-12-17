
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
		// ����ļ������Ƿ���Դ���
		bool CheckFileType(const VSString & path)
		{
			return path.GetSubStringIndex(VSResource::GetFileSuffix(GetResourceType()), 1) != -1;
		}
		// �����ļ������¼�
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
		/// �����ļ������¼�
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

		// ���ü���Ŀ¼
		void SetDirectory(const VSString& s);
		// ȡ�ü���Ŀ¼
		const VSString & GetDirectory() const;

		// �����Ƿ�ʹ�����·��
		void SetUseRelativePath(bool b);
		// �Ƿ�ʹ�����·��
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

		/// ��ʼ����
		bool Open();
		/// ����
		void Update(double dAppTime);
		/// ��������
		void Close();

		/// ���ü���Ŀ¼
		void SetMonitorDirectory(const VSString& path);

		/// ע���ļ����ʹ���ӿ�
		void RegisterHandler(VSResourceChangedHandler* handler);
		/// ɾ����Ӧ����ӿ�
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
