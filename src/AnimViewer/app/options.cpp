#include "lucPCH.h"
#include "options.hpp"
#include <stdio.h>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include "platform/path.h"


// TODO:
// a wapper of file leading functions. 
// Support to read file from different source, like packed or unpacked file
//class FileLoader
//{
//public:
//    FileLoader(const char* const filename);
//    ~FileLoader();
//
//    std::string ReadString(const char* const keyname);
//private:
//    std::string m_filename;
//};

const std::string Options::m_configFileName("configure.txt");

bool Options::LoadConfig()
{
    const int LINE_BUFFER_SIZE = 256;
    char linebuffer[LINE_BUFFER_SIZE];

    FILE* fp = fopen(m_configFileAbsPath.c_str(), "r");
    if (NULL == fp)
        return false;

    bool noError = true;
    while ( !feof(fp) )
    {
        fgets(linebuffer, LINE_BUFFER_SIZE, fp);
        std::string line(linebuffer);
        size_t returnChar = line.find('\n');
        if (returnChar != std::string::npos)
            line.erase(returnChar);

        if (line.empty())
            continue;

        size_t separator = line.find('=');
        if (separator < line.size())
        {
            std::string key(line.begin(), line.begin() + separator);
            std::string str(line.begin() + separator + 1, line.end());

            boost::trim(key);
            boost::trim(str);

            if (0 == key.compare("ROOT"))
            {
                mRootFolder = str;

                Luc::PathInfoSingleton::Instance().CPSetCurrentDirectory(str.c_str());
                noError = true;
            }
            else if (0 == key.compare("input_scene"))
            {
                input_filename = str;
                noError &= true;
            }
            else if (0 == key.compare("screen_width"))
            {
                width = atoi(str.c_str());
                noError &= true;
            }
            else if (0 == key.compare("screen_height"))
            {
                height = atoi(str.c_str());
                noError &= true;
            }
            else if (0 == key.compare("window_title"))
            {
                mWindowTitile = str;
                noError &= true;
            }
            else if (0 == key.compare("fixed_fps"))
            {
                mFps = static_cast<float>(atof(str.c_str()));
                noError &= true;
            }
        }
    }
    if (input_filename.empty())
        noError = false;

    return noError;
}

Options::Options() : m_bInitialized(false), m_FCNHandle(Luc::FileChangeNotification::INVALID_HANDLE)
{
    if (false == m_bInitialized)
    {
        m_configFileAbsPath = AddWatchFile(m_configFileName.c_str());

        // don't load before add to FileChangeNotification system. 
        LoadConfig();
    }
}

Options::~Options()
{
    SafeCheckAndRemoveWatchFile();
}

void Options::OnFileChangeNotification()
{
    LoadConfig();
}
