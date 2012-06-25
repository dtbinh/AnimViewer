#ifndef CORE_CACHE_FILECHANGENOTIFICATION_IMANAGER_H
#define CORE_CACHE_FILECHANGENOTIFICATION_IMANAGER_H

#include "FileChangeNotification.h"

namespace Luc
{
namespace FileChangeNotification
{
    class IManager
    {
    public:
        //!
        //! Add a file and callback to FileChangeNotification system.
        //!
        virtual FCNHandle   Add(const std::string& filename, FCNCallback callback) = 0;

        //!
        //! Remove a file from FileChangeNotification system.
        //!
        virtual void        Remove(FCNHandle handle) = 0;

        //!
        //! Update FileChangeNotification system.
        //! This function will add pending-add files, remove pending-remove 
        //! files, and reload changed files.
        //! 
        virtual void        Update() = 0;

        //!
        //! Force to reload all files.
        //! 
        virtual void        ReloadAll() = 0;
    };
}
}

#endif // CORE_CACHE_FILECHANGENOTIFICATION_IMANAGER_H
