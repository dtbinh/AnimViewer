#include "lucPCH.h"
#include "files/fileChangeNotification/FCNManager.h"
#include "files/fileChangeNotification/DirectoryWatch.h"
#include "platform/path.h"
#include "platform/error.h"
#include "files/fileutils.h"
#include "math/mathUtils.h"
#include <boost/thread.hpp>
#include <Windows.h>


namespace Luc
{
namespace FileChangeNotification
{


    //////////////////////////////////////////////////////////////////////////
    // 
    //////////////////////////////////////////////////////////////////////////

    //!
    //! add a handler to listen a file change
    //! @param filename Name of the file
    //! @param handler  Callback handler to file change.
    //! @return         Handle of the file.
    //! 
    // TODO: change to hashed string is better
    Luc::FileChangeNotification::FCNHandle AddFileChangeNotificationHandler( const std::string& filepath, FCNCallback callback )
    {
        std::string fullpath = filepath;
        if (!Luc::IsNotAbsolutePath(filepath))
        {
            Luc::ConvertToAbsolutePath(filepath, fullpath);
        }

        return FileHandlerManagerSingleton::Instance().Add(fullpath, callback);
    }

    //!
    //! Remove a handler for a file.
    //! @param handle   Handle of the file.
    //! 
    void RemoveFileChangeNotificationHandler( const FCNHandle handle )
    {
        FileHandlerManagerSingleton::Instance().Remove(handle);
    }


    void UpdateFileChangeNotificationSystem()
    {
        FileHandlerManagerSingleton::Instance().Update();
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // ICallback
    // 
    //////////////////////////////////////////////////////////////////////////
    std::string ICallback::AddWatchFile(const char* filename)
    {
        // if already watching, remove old one
        SafeCheckAndRemoveWatchFile();

        // real adding file
        std::string absPath;
        Luc::ConvertToAbsolutePath(filename, absPath);
        m_FCNHandle = AddFileChangeNotificationHandler(absPath, this);
        return absPath;
    }

    void ICallback::SafeCheckAndRemoveWatchFile()
    {
        if (INVALID_HANDLE != m_FCNHandle)
        {
            RemoveFileChangeNotificationHandler(m_FCNHandle);
            m_FCNHandle = INVALID_HANDLE;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // FCNManagerImpl
    // 
    //////////////////////////////////////////////////////////////////////////
    class FCNManagerImpl
    {
    public:
        FCNManagerImpl();
        ~FCNManagerImpl();

        //!
        //! Add a new file to FileChangeNotification. If this file is already in 
        //! the list, don't add but directly return handle.
        //! @param[in]  filepath    
        //! @param[in]  callback    Pointer of an derived instance of 
        //!                         Luc::FileChangeNotification::ICallback class.
        //! @return                 Handle of new added file.
        //! 
        FCNHandle   Add(const std::string& filepath, FCNCallback callback);

        void        Remove(FCNHandle handle);

        void        Update();
        void        Watch();
        void        ReloadAll();

    private: 
        FCNHandle FormHandle(uint32_t dirIndex, uint32_t fileIndex)
        {
            assert(dirIndex < MAX_UINT_16 && fileIndex < MAX_UINT_16);
            return ((dirIndex << 16) + fileIndex);
        }

        void      ParseHandle(FCNHandle handle, uint32_t& dirIndex, uint32_t& fileIndex)
        {
            dirIndex = (handle & 0xFFFF0000) >> 16;
            fileIndex = handle & 0xFFFF;
        }

        uint32_t FindOrAddDirectoryWatch( const std::string& directory );

        bool FindDirectoryWatch( const Luc::Common::HashedString& directory, uint32_t& dirIndex );


    private:
        typedef std::vector< boost::shared_ptr<DirectoryWatch> > DirectoryWatchVector;    // use vector since it is fast in runtime check, but slower when add/remove.
        DirectoryWatchVector m_directoryWatches;
        std::vector<HANDLE> m_directoryHandles; 


        static const uint32_t INVALID_INDEX;
        static const uint32_t MAX_DIR_INDEX;
    };
    const uint32_t FCNManagerImpl::INVALID_INDEX = MAX_UINT_16;
    const uint32_t FCNManagerImpl::MAX_DIR_INDEX = MAX_UINT_16;

    FCNManagerImpl::FCNManagerImpl() 
    {
        //boost::thread FCNThread(*this);
        boost::thread FCNThread(&FCNManagerImpl::Watch, this);
    }

    FCNManagerImpl::~FCNManagerImpl()
    {
        m_directoryWatches.clear();
    }

    FCNHandle FCNManagerImpl::Add( const std::string& filepath, FCNCallback callback )
    {
        //std::string curDirectory = PathInfoSingleton::Instance().CPGetCurrentDirectory();

        std::string directory;
        std::string filename;
        ParseFilePath(filepath, directory, filename);

        uint32_t dirIndex = FindOrAddDirectoryWatch(directory);
        uint32_t fileIndex = m_directoryWatches[dirIndex]->AddFile(filepath, callback);
        return FormHandle(dirIndex, fileIndex);
    }

    void FCNManagerImpl::Remove( FCNHandle handle )
    {
        uint32_t dirIndex ( INVALID_INDEX );
        uint32_t fileIndex( INVALID_INDEX );
        ParseHandle(handle, dirIndex, fileIndex);

        assert (dirIndex < m_directoryWatches.size());
        m_directoryWatches[dirIndex]->Remove(fileIndex);
    }

    void FCNManagerImpl::Update()
    {
        DirectoryWatchVector::iterator itEnd    = m_directoryWatches.end();
        DirectoryWatchVector::iterator it       = m_directoryWatches.begin();
        for (; it != itEnd; ++it)
        {
            DirectoryWatchShare_ptr shDirectoryWatch = *it;
            if (shDirectoryWatch->IsDirty())
                shDirectoryWatch->OnDirectoryChangeNotification();
        }

    }

    void FCNManagerImpl::Watch()
    {
        while (true) 
        { 
            DWORD handleCount = static_cast<DWORD>(m_directoryHandles.size());
            if (0 == handleCount)
                boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

            // TODO: according to Windows API, this array can only handle 80 directories.
            DWORD waitStatus = WaitForMultipleObjects(
                handleCount, &m_directoryHandles[0], FALSE, 1000); 

            if (WAIT_FAILED == waitStatus)
            {
            }
            else if (WAIT_TIMEOUT == waitStatus)
            {
            }
            else 
            {
                int index = waitStatus - WAIT_OBJECT_0;

                m_directoryWatches[index]->MarkDirty();

                // restart notification
                if ( FindNextChangeNotification(m_directoryHandles[index]) == FALSE )
                {
                    printf("\n ERROR: FindNextChangeNotification function failed.\n");
                    ExitProcess(GetLastError()); 
                }
            }
        }

    }

    void FCNManagerImpl::ReloadAll()
    {
        DirectoryWatchVector::iterator itEnd    = m_directoryWatches.end();
        DirectoryWatchVector::iterator it       = m_directoryWatches.begin();
        for (; it != itEnd; ++it)
        {
            (*it)->ReloadAll();
        }
    }



    uint32_t FCNManagerImpl::FindOrAddDirectoryWatch( const std::string& directory )
    {
        uint32_t dirIndex( INVALID_INDEX );

        // if doesn't exist this directory, add a new directory watch 
        const Luc::Common::HashedString hashedDirectory(directory);
        if (false == FindDirectoryWatch(hashedDirectory, dirIndex))
        {
            m_directoryWatches.push_back(DirectoryWatchShare_ptr(new DirectoryWatch(hashedDirectory)));
            dirIndex = static_cast<uint32_t>(m_directoryWatches.size() - 1);
            assert(dirIndex < MAX_UINT_16);

            //
            // Watch the directory for file creation and deletion. 
            //
            HANDLE handle = FindFirstChangeNotification( 
                directory.c_str(),                    // directory to watch 
                FALSE,                           // do not watch subtree 
                FILE_NOTIFY_CHANGE_LAST_WRITE    // watch file name changes 
                );

            if (INVALID_HANDLE_VALUE == handle || NULL == handle)
            {
                std::string message ("Failed to FindFirstChangeNotification for file:");
                message += directory + ". ";

                std::string errorMessage;
                message += Luc::GetErrorMessage(errorMessage);
                Luc::LOG(message.c_str());
                assert(false && message.c_str());
            }
            else
                m_directoryHandles.push_back(handle);
        }

        return dirIndex;
    }

    bool FCNManagerImpl::FindDirectoryWatch( const Luc::Common::HashedString& directory, uint32_t& dirIndex )
    {
        assert (m_directoryWatches.size() < INVALID_INDEX);

        // check all existing directories' path
        DirectoryWatchVector::iterator itBegin  = m_directoryWatches.begin();
        DirectoryWatchVector::iterator itEnd    = m_directoryWatches.end();
        DirectoryWatchVector::iterator it       = itBegin;
        for (; it != itEnd; ++it)
        {
            if (directory == (*it)->GetPath())
            {
                // simply add to directory watch
                dirIndex = static_cast<uint32_t>(it - itBegin);
                return true;
            }
        }

        dirIndex = INVALID_INDEX;
        return false;
    }



    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    FCNManager::FCNManager() : m_pImpl(new FCNManagerImpl())
    {

    }

    FCNManager::~FCNManager()
    {
        m_pImpl.reset();
    }

    Luc::FileChangeNotification::FCNHandle FCNManager::Add( const std::string& filepath, FCNCallback callback )
    {
        return m_pImpl->Add(filepath, callback);
    }

    void FCNManager::Remove( FCNHandle handle )
    {
        m_pImpl->Remove(handle);
    }

    void FCNManager::Update()
    {
        m_pImpl->Update();
    }

    void FCNManager::ReloadAll()
    {
        m_pImpl->ReloadAll();
    }

}
}
