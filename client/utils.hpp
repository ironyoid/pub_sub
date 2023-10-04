#pragma once
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

namespace Utils {
    bool GetPortFromStr (std::string &str, uint16_t &port);
} // namespace Utils

namespace ErrorCodes {
    typedef enum {
        eStatus_Ok = 0,
        eStatus_WrongArgsNum,
        eStatus_GeneralError,
    } eStatus_t;
} // namespace ErrorCodes
