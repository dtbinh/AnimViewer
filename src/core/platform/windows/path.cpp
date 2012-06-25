#include "lucPCH.h"
#include "platform/path.h"
#include "platform/error.h"
#include "log/log.h"
#include <windows.h>
#include <limits>

namespace Luc {

PathInfo::PathInfo() : m_initialized(false)
{
    DoGetCurrentDirectory();
}

PathInfo::~PathInfo()
{
}

void PathInfo::CPSetCurrentDirectory(const char * directory)
{
    ::SetCurrentDirectory(directory);
    DoGetCurrentDirectory();
}

const std::string& PathInfo::CPGetCurrentDirectory() const
{
    if (false == m_initialized)
    {
        assert(false && "Error: PathInfo singleton is not initialized!");
    }

    return m_currentDirectory;
}

void PathInfo::DoGetCurrentDirectory()
{
    char* currentDirectory = new char[512];
    assert (NULL != currentDirectory);

    memset(currentDirectory, 0, 512);
    DWORD rslt = ::GetCurrentDirectory(512, currentDirectory);
    if (0 == rslt)
    {
        m_initialized = false;
        std::string errorMessage;
        LOG(GetErrorMessage(errorMessage).c_str());
    }
    else
    {
        m_currentDirectory = currentDirectory;
        m_initialized = true;
    }
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//bool StartDirectoryNotification( const char* directory, bool watchSubtree )
//{
//
//}

}
