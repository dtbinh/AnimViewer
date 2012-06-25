#ifndef CORE_PLATFORM_PATH_H
#define CORE_PLATFORM_PATH_H

namespace Luc {

    class PathInfo
    {
    public:
        PathInfo();
        ~PathInfo();

        void CPSetCurrentDirectory(const char * directory);
        const std::string& CPGetCurrentDirectory() const;

    private:
        void DoGetCurrentDirectory();

    private:
        std::string m_currentDirectory;
        bool        m_initialized;
    };

    typedef Loki::SingletonHolder<PathInfo> PathInfoSingleton;


    //bool StartDirectoryNotification(const char* directory, bool watchSubtree);
}

#endif // CORE_PLATFORM_PATH_H
