#ifndef CORE_FILES_FILECHANGENOTIFICATION_DIRECTORYWATCH_H
#define CORE_FILES_FILECHANGENOTIFICATION_DIRECTORYWATCH_H

#include "FileChangeNotification.h"
#include "utils/hashedString.h"

namespace Luc
{
namespace FileChangeNotification
{
    class FileWatch;

    class DirectoryWatch
    {
    public:
        DirectoryWatch(const std::string& directory);
        DirectoryWatch(const Luc::Common::HashedString& directory);
        ~DirectoryWatch();

        uint32_t AddFile(const std::string& filename, FCNCallback callback);
        void Remove(FCNHandle handle);
        void ReloadAll();
        bool IsEmpty()  { return m_fileWatches.empty(); };

        //!
        //! When this directory has change notification, need to figure out 
        //! which file was changed, then reload it.
        //! 
        void OnDirectoryChangeNotification();
        void Update();

        //!
        //! Dirty flag
        //! 
        void MarkDirty()    { m_isDirty = true;  }
        void ClearDirty()   { m_isDirty = false; }
        bool IsDirty()      { return m_isDirty;  }


        const Luc::Common::HashedString& GetPath() const {return m_directory;};

    private:
        typedef std::vector< boost::shared_ptr<FileWatch> > FileWatchVector;    // use vector since it is fast in runtime check, but slower when add/remove.
        FileWatchVector m_fileWatches;

        Luc::Common::HashedString   m_directory;    // use hash to accelerate add and remove operation
        bool                        m_isDirty;

        static const uint32_t INVALID_INDEX;
        static const uint32_t MAX_DIR_INDEX;
    };

    typedef boost::shared_ptr<DirectoryWatch> DirectoryWatchShare_ptr;

    //inline bool operator==(DirectoryWatch& lhv, DirectoryWatch& rhv)    {return lhv.GetPath() == rhv.GetPath(); }
    //inline bool operator< (DirectoryWatch& lhv, DirectoryWatch& rhv)    {return lhv.GetPath() <  rhv.GetPath(); };
    //inline bool operator> (DirectoryWatch& lhv, DirectoryWatch& rhv)    {return lhv.GetPath() >  rhv.GetPath(); };

}
}

#endif // CORE_FILES_FILECHANGENOTIFICATION_DIRECTORYWATCH_H