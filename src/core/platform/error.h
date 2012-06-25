#ifndef CORE_PLATFORM_ERROR_H
#define CORE_PLATFORM_ERROR_H

namespace Luc
{
    //!
    //! Get error message by translate Windows GetLastError code.
    //! @param[in][out] message     String instance to hold translated message.
    //! @return                     Reference of input message parameter.
    //! 
    std::string& GetErrorMessage(std::string& message);
}

#endif // CORE_PLATFORM_ERROR_H
