#ifndef _UTILS_H__
#define _UTILS_H__
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

namespace Utils {
    bool GetPortFromStr (const std::string &str, uint16_t &port);
} // namespace Utils

namespace ErrorCodes {
    typedef enum {
        eStatus_Ok = 0,
        eStatus_WrongArgsNum,
        eStatus_GeneralError,
        eStatus_ElementExistsError,
    } eStatus_t;
} // namespace ErrorCodes

#endif /*_UTILS_H__*/
