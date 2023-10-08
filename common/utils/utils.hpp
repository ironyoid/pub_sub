#ifndef _UTILS_H__
#define _UTILS_H__
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

namespace ErrorCodes {
    typedef enum {
        eStatus_Ok = 0,
        eStatus_WrongArgsNum,
        eStatus_GeneralError,
        eStatus_ElementExistsError,
        eStatus_LostConnection,
    } eStatus_t;
} // namespace ErrorCodes

namespace Utils {
    bool GetPortFromStr (const std::string &str, uint16_t &port);
    bool CheckAddrArgument (std::vector<std::string> args);
} // namespace Utils

#endif /*_UTILS_H__*/
