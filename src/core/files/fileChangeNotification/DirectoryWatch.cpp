#include "lucPCH.h"
#include "DirectoryWatch.h"
#include "FileWatch.h"
#include <Windows.h>

namespace Luc
{
namespace FileChangeNotification
{
    //////////////////////////////////////////////////////////////////////////
    // constructors/destructors
    //////////////////////////////////////////////////////////////////////////
    DirectoryWatch::DirectoryWatch( const std::string& directory ) : m_directory(directory), m_isDirty(false)
    {

    }

    DirectoryWatch::DirectoryWatch( const Luc::Common::HashedString& directory ) : m_directory(directory), m_isDirty(false)
    {

    }

    DirectoryWatch::~DirectoryWatch()
    {
        m_fileWatches.clear();
    }

    uint32_t DirectoryWatch::AddFile(const std::string& filename, FCNCallback callback)
    {
        Luc::Common::HashedString hashedFileName(filename);

        // check if already exist
        FileWatchVector::iterator itBegin = m_fileWatches.begin();
        FileWatchVector::iterator itEnd   = m_fileWatches.end();
        FileWatchVector::iterator it      = itBegin;
        for (; it!= itEnd; ++it)
        {
            FileWatchShared_ptr shFileWatch = *it;
            if ( hashedFileName == shFileWatch->GetFileName() )
            {
                return static_cast<uint32_t>(it - itBegin);
            }
        }
        // if not exist, add to file watch array
        FileWatchShared_ptr newFileWatch(new FileWatch(filename, callback));
        m_fileWatches.push_back(newFileWatch);
        return static_cast<uint32_t>(m_fileWatches.size() - 1);
    }

    void DirectoryWatch::Remove( FCNHandle handle )
    {
        assert (handle < m_fileWatches.size());
        // we cannot erase this element, otherwise all following element handles
        // will not be valid anymore.
        m_fileWatches[handle]->Deactivate();
    }

    void DirectoryWatch::ReloadAll()
    {
        FileWatchVector::iterator itEnd   = m_fileWatches.end();
        FileWatchVector::iterator it      = m_fileWatches.begin();
        for (; it!= itEnd; ++it)
        {
            FileWatchShared_ptr shFileWatch = *it;
            shFileWatch->InvokeCallback();
        }
    }

    void DirectoryWatch::OnDirectoryChangeNotification()
    {
        FileWatchVector::iterator itEnd   = m_fileWatches.end();
        FileWatchVector::iterator it      = m_fileWatches.begin();
        for (; it!= itEnd; ++it)
        {
            FileWatchShared_ptr shFileWatch = *it;
            // get write time
            shFileWatch->OnDirectoryChangeNotification();
        }
        ClearDirty();
    }

    void DirectoryWatch::Update()
    {
        if (IsDirty())
        {
            OnDirectoryChangeNotification();
        }
    }

}
}