/// options.h
/// Defines application options.
///
#ifndef APPLICATION_OPTIONS_H
#define APPLICATION_OPTIONS_H

#include "files/fileChangeNotification/FileChangeNotification.h"

/**
 * Struct of the program options.
 */
class Options : Luc::FileChangeNotification::ICallback
{
public:
    Options();;
    ~Options();;

    bool LoadConfig();
    virtual void OnFileChangeNotification();
    
    std::string input_filename;
    std::string output_filename;
    std::string mRootFolder;
    std::string mWindowTitile;
    
    int width, height; // window dimensions
    float mFps;

private:
    bool m_bInitialized;
    Luc::FileChangeNotification::FCNHandle m_FCNHandle;

    static const std::string    m_configFileName;
    std::string                 m_configFileAbsPath;
    
};

//Loki::SingletonHolder<Options>

#endif // APPLICATION_OPTIONS_H
