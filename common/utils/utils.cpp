#include "utils.hpp"
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using boost::bad_lexical_cast;
using boost::lexical_cast;

namespace Utils {
    std::optional<uint16_t> GetPortFromStr (const std::string &str) {
        try {
            int32_t port_32 = lexical_cast<int32_t>(str);
            if(port_32 < 1 || port_32 > std::numeric_limits<uint16_t>::max()) {
                throw bad_lexical_cast();
            }
            uint16_t port = static_cast<uint16_t>(port_32);
            return std::optional<uint16_t>{ port };

        } catch(bad_lexical_cast &) {
        }
        return {};
    }

    bool CheckAddrArgument (const std::vector<std::string> args) {
        bool ret = false;
        if(args.size() == 1) {
            try {
                boost::asio::ip::address::from_string(args[0]);
                ret = true;
            } catch(const std::exception &e) {
            }
        }
        return ret;
    }

    std::string StreamBufToString (boost::asio::streambuf &buffer) {
        std::string msg;
        std::stringstream ss;
        ss << &buffer;
        ss.flush();
        msg = ss.str();
        return msg;
    }
} // namespace Utils