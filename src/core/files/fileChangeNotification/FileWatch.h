#ifndef CORE_FILES_FILECHANGENOTIFICATION_FILEWATCH_H
#define CORE_FILES_FILECHANGENOTIFICATION_FILEWATCH_H

#include "FileChangeNotification.h"
#include "utils/hashedString.h"

namespace Luc
{
    namespace FileChangeNotification
    {
        class FileWatchImpl;

        class FileWatch
        {
        public:
            FileWatch(const std::string& filename, FCNCallback callback);
            FileWatch(const Luc::Common::HashedString& filename, FCNCallback callback);
            ~FileWatch();

            void InvokeCallback();
            const Luc::Common::HashedString& GetFileName() const;

            void Deactivate();
            void Activate();
            bool IsActivated() const;
            //!
            //! The directory was changed, check myself if I was changed.
            //! 
            void OnDirectoryChangeNotification();

        private:
            boost::shared_ptr<FileWatchImpl> m_pImpl;
        };

        typedef boost::shared_ptr<FileWatch> FileWatchShared_ptr;
    }
}

#endif // CORE_FILES_FILECHANGENOTIFICATION_FILEWATCH_H
