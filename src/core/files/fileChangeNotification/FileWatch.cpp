#include "lucPCH.h"
#include "FileWatch.h"
#include <Windows.h>

namespace Luc
{
namespace FileChangeNotification
{
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    class FileWatchImpl
    {
    public:
        FileWatchImpl(const std::string& filename, FCNCallback callback) : 
            m_filename(filename), m_callback(callback), m_activated(true) 
        {
            m_fileLastWriteTime = GetFileLastWriteTime(filename);
        }
        FileWatchImpl(const Luc::Common::HashedString& filename, FCNCallback callback) :
            m_filename(filename), m_callback(callback), m_activated(true)
        {
            m_fileLastWriteTime = GetFileLastWriteTime(filename.GetString());
        }

        ~FileWatchImpl() { Deactivate(); }

        void InvokeCallback()   { m_callback->OnFileChangeNotification(); }

        const Luc::Common::HashedString& GetFileName() const { return m_filename; }

        void Deactivate()   { m_activated = false;  }
        void Activate()     { m_activated = true;   }
        bool IsActivated() const    { return m_activated; }
        void OnDirectoryChangeNotification();

    private:
        FILETIME GetFileLastWriteTime( const std::string &filename ) 
        {
            WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
            if (0 == GetFileAttributesEx( filename.c_str(), GetFileExInfoStandard, &fileAttributes ))
            {
                LOG("Failed to load file time");
            }
            return fileAttributes.ftLastWriteTime;
        }

    private:
        Luc::Common::HashedString   m_filename; // use hashed string to accelerate add/remove file to directory watch. This is absolute path
        FCNCallback                 m_callback;
        bool                        m_activated;
        FILETIME                    m_fileLastWriteTime;
        // TODO: last file attribute data
    };

    // if 
    void FileWatchImpl::OnDirectoryChangeNotification()
    {
        FILETIME fileLastWriteTime =  GetFileLastWriteTime(m_filename.GetString());
        if (fileLastWriteTime.dwHighDateTime != m_fileLastWriteTime.dwHighDateTime ||
            fileLastWriteTime.dwLowDateTime  != m_fileLastWriteTime.dwLowDateTime)
        {
            InvokeCallback();
            m_fileLastWriteTime = fileLastWriteTime;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // FileWatch
    //////////////////////////////////////////////////////////////////////////
    FileWatch::FileWatch( const std::string& filename, FCNCallback callback ) :
        m_pImpl(new FileWatchImpl(filename, callback))
    {}

    FileWatch::FileWatch( const Luc::Common::HashedString& filename, FCNCallback callback ) :
        m_pImpl(new FileWatchImpl(filename, callback))
    {}

    FileWatch::~FileWatch() { m_pImpl.reset(); }

    void FileWatch::InvokeCallback() { m_pImpl->InvokeCallback(); }

    const Luc::Common::HashedString& FileWatch::GetFileName() const
    {
        return m_pImpl->GetFileName();
    }

    void FileWatch::Deactivate()
    {
        m_pImpl->Deactivate();
    }

    void FileWatch::Activate()
    {
        m_pImpl->Activate();
    }

    bool FileWatch::IsActivated() const
    {
        return m_pImpl->IsActivated();
    }

    void FileWatch::OnDirectoryChangeNotification()
    {
        m_pImpl->OnDirectoryChangeNotification();
    }

}
}
