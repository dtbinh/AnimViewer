#include "lucPCH.h"
#include "files/fileutils.h"
#include "platform/path.h"

namespace Luc
{
bool ParseFilePath(const std::string& filepath, std::string& dir, std::string& filename)
{
    size_t pos = filepath.find_last_of('\\');

    if (std::string::npos != pos) // if there is directory
    {
        dir = filepath.substr(0, pos);
        filename = filepath.substr(pos+1);
        return true;
    }
    else    // no directory in the path.
    {
        //assert(false);  // throw assert for now. Not sure what we should do for it.
        dir = ".";
        filename = filepath;
        return false;
    }
}

bool IsNotAbsolutePath( const std::string& filepath )
{
    size_t pos = filepath.find_last_of(':');
    return (std::string::npos == pos);
}

std::string& ConvertToAbsolutePath( const std::string& filepath, std::string& fullpath )
{
    if (IsNotAbsolutePath(filepath))
    {
        const std::string& currentDirectory = PathInfoSingleton::Instance().CPGetCurrentDirectory();
        fullpath = currentDirectory + "\\" + filepath;
    }
    return fullpath;
}

}

