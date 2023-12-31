#ifndef _UTILS_H__
#define _UTILS_H__
#include <string>
#include <optional>
#include "boost/asio.hpp"

namespace ErrorStatus {
    enum class eStatus_t {
        Ok = 0,
        WrongArgsNum,
        GeneralError,
        ElementExistsError,
        LostConnection,
        WrongPort,
        PortAlreadyInUse,
        ConnectionRefused,
        ConnectionAlreadyExist
    };
    const std::string ToString (ErrorStatus::eStatus_t &status);
} // namespace ErrorStatus

namespace Utils {
    std::optional<uint16_t> GetPortFromStr (const std::string &str);
    bool CheckAddrArgument (std::vector<std::string> args);
    const std::string StreamBufToString (boost::asio::streambuf &buffer);
} // namespace Utils

namespace HelpString {
    void PrintHelp (void);
} // namespace HelpString

#endif /*_UTILS_H__*/
