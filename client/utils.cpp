#include "utils.hpp"

using boost::bad_lexical_cast;
using boost::lexical_cast;

namespace Utils {
    bool GetPortFromStr (std::string &str, uint16_t &port) {
        bool ret = false;
        try {
            int32_t port_32 = lexical_cast<int32_t>(str);
            if(port_32 < 0 || port_32 > UINT16_MAX) {
                throw bad_lexical_cast();
            }
            port = static_cast<uint16_t>(port_32);
            ret = true;

        } catch(bad_lexical_cast &) {
        }
        return ret;
    }
} // namespace Utils