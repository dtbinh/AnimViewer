#ifndef CORE_CACHE_FILECHANGENOTIFICATION_FILEHANDLERMANAGER_H
#define CORE_CACHE_FILECHANGENOTIFICATION_FILEHANDLERMANAGER_H

#include <map>
#include <vector>
#include "IManager.h"




namespace Luc
{
namespace FileChangeNotification
{

    class DirectoryWatch;
    class FCNManagerImpl;


    // TODO: use pImpl pattern.

    class FCNManager : public IManager
    {
    public:
        FCNManager();
        ~FCNManager();

        //!
        //! Add a new file to FileChangeNotification. If this file is already in 
        //! the list, don't add but directly return handle.
        //! @param[in]  filepath    
        //! @param[in]  callback    Pointer of an derived instance of 
        //!                         Luc::FileChangeNotification::ICallback class.
        //! @return                 Handle of new added file.
        //! 
        virtual FCNHandle   Add(const std::string& filepath, FCNCallback callback);
        virtual void        Remove(FCNHandle handle);
        virtual void        Update();
        virtual void        ReloadAll();

    private:
        boost::shared_ptr<FCNManagerImpl> m_pImpl;
    };

    typedef Loki::SingletonHolder<FCNManager> FileHandlerManagerSingleton;
}
}
#endif // CORE_CACHE_FILECHANGENOTIFICATION_FILEHANDLERMANAGER_H
