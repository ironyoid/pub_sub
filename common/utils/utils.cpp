#include "utils.hpp"
#include <_types/_uint16_t.h>
#include <boost/asio.hpp>

using boost::bad_lexical_cast;
using boost::lexical_cast;

namespace Utils {
    std::optional<uint16_t> GetPortFromStr (const std::string &str) {
        uint16_t port = 0;
        try {
            int32_t port_32 = lexical_cast<int32_t>(str);
            if(port_32 < 1 || port_32 > UINT16_MAX) {
                throw bad_lexical_cast();
            }
            port = static_cast<uint16_t>(port_32);
            return std::optional<uint16_t>{ port };

        } catch(bad_lexical_cast &) {
        }
        return {};
    }

    bool CheckAddrArgument (const std::vector<std::string> args) {
        bool ret = false;
        if(args.size() == 1) {
            try {
                boost::asio::ip::address addr = boost::asio::ip::address::from_string(args[0]);
                ret = true;
            } catch(std::exception &e) {
            }
        }
        return ret;
    }
} // namespace Utils