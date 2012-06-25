#include "lucPCH.h"
#include "platform/error.h"
#include <windows.h>

namespace Luc
{
std::string& GetErrorMessage(std::string& message)
{
    char* lpMsgBuf=NULL;
    DWORD errorCode = GetLastError();
    DWORD Size = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
        NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL
    );
    if (lpMsgBuf && Size > 0)
    {
        // Remove \n
        for (DWORD i = 0; i < Size; ++i)
        {
            if ((lpMsgBuf[i] == '\n') || (lpMsgBuf[i] == '\r'))
            {
                lpMsgBuf[i] = '\0';
            }
        }

        message = lpMsgBuf;
        LocalFree(lpMsgBuf);
    }
    else
    {
        if (errorCode)
        {
            char tmpMsg[256];
            sprintf(tmpMsg, "FormatMessage returned an error:%d.", Size);
            message = tmpMsg;
        }
        else
        {
            message = std::string("Unknown error code");
        }
    }

    return message;
}
}
