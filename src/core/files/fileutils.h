#ifndef CORE_FILES_FILESUTILS_H
#define CORE_FILES_FILESUTILS_H

namespace Luc
{
    //!
    //! Parse file path to two parts: directory and filename.
    //! @param[in]  filepath    Full file path
    //! @param[out] directory   
    //! @param[out] filename
    //! @return false if cannot find a directory in given input parameter.
    //! 
    bool ParseFilePath(const std::string& filepath, std::string& dir, std::string& filename);

    bool IsNotAbsolutePath(const std::string& filepath);

    //!
    //! ConvertToAbsolutePath. 
    //! If the path is already absolute path, directly return; otherwise convert 
    //! to absolute path.
    //! 
    //! @param[in]  filepath    
    //! @param[out] fullpath    Output absolute path. This can be the same string as input filepath.
    //! @return                 Reference of fullpath.
    //! 
    std::string& ConvertToAbsolutePath(const std::string& filepath, std::string& fullpath);

}

#endif // CORE_FILES_FILESUTILS_H
