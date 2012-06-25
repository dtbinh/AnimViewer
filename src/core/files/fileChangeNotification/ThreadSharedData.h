#ifndef CORE_FILES_FILECHANGENOTIFICATION_THREADSHAREDDATA_H
#define CORE_FILES_FILECHANGENOTIFICATION_THREADSHAREDDATA_H

#include "FileChangeNotification.h"

namespace Luc
{
namespace FileChangeNotification
{
    class ThreadSharedData
    {
    public:
        ThreadSharedData();
        ~ThreadSharedData();

    private:
        std::vector<FileWatch*> m_changedFiles;
        std::vector<FileWatch*> m_pendingAddFiles;
        std::vector<FCNHandle> m_pendingRemoveFiles;
        bool                    m_isTerminated;
        // TODO: mutex
    };
}
}
#endif // CORE_FILES_FILECHANGENOTIFICATION_THREADSHAREDDATA_H
