// fileChangeNotification.h
// 
// This file is the public interfaces of file change notification system.
// 
// author: Yantong Liu
// date  : 4/12/2012
//////////////////////////////////////////////////////////////////////////

#ifndef CORE_CACHE_FILECHANGENOTIFICATION_H
#define CORE_CACHE_FILECHANGENOTIFICATION_H

#include "utils/hashedString.h"

namespace Luc
{
namespace FileChangeNotification
{
    // forward declaration
    class ICallback;

    typedef uint32_t            FCNHandle;  // a short, simple name
    typedef ICallback* const    FCNCallback;// a short, simple name

    const FCNHandle INVALID_HANDLE = MAX_UINT_32;

    //!
    //! Update FileChangeNotification system.
    //! 
    void UpdateFileChangeNotificationSystem();

    //! 
    //! @class ICallback
    //! Class provided basic file change notification functions
    //! 
    class ICallback
    {
    public:
        ICallback() :  m_FCNHandle(INVALID_HANDLE) {}
        virtual ~ICallback()        { SafeCheckAndRemoveWatchFile(); }

        std::string AddWatchFile(const char* filename);
        void SafeCheckAndRemoveWatchFile();

        virtual void OnFileChangeNotification() = 0;

    private:
        FCNHandle m_FCNHandle;
    };

}
}

#endif // CORE_CACHE_FILECHANGENOTIFICATION_H
