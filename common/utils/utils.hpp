#ifndef _UTILS_H__
#define _UTILS_H__
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <optional>

namespace ErrorCodes {
    typedef enum {
        eStatus_Ok = 0,
        eStatus_WrongArgsNum,
        eStatus_GeneralError,
        eStatus_ElementExistsError,
        eStatus_LostConnection,
        eStatus_WrongPort,
        eStatus_PortAlreadyInUse,
        eStatus_ConnectionRefused,
    } eStatus_t;
} // namespace ErrorCodes

namespace Utils {
    std::optional<uint16_t> GetPortFromStr (const std::string &str);
    bool CheckAddrArgument (std::vector<std::string> args);
} // namespace Utils

#endif /*_UTILS_H__*/
