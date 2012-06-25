#ifndef CORE_FILES_FILECHANGENOTIFICATION_DIRECTORYWATCHTHREAD_H
#define CORE_FILES_FILECHANGENOTIFICATION_DIRECTORYWATCHTHREAD_H

#include "FileChangeNotification.h"
#include "ThreadSharedData.h"

namespace Luc
{
namespace FileChangeNotification
{

    class DirectoryWatchThread
    {
    public:
        DirectoryWatchThread();
        ~DirectoryWatchThread();
    private:
        ThreadSharedData* m_sharedData;
    };
}
}
#endif // CORE_FILES_FILECHANGENOTIFICATION_DIRECTORYWATCHTHREAD_H
