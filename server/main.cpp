#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <array>
#include "tcp_server.hpp"
#include "logs.hpp"
#include "utils.hpp"

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;
    if(2 == argc) {
        auto port = Utils::GetPortFromStr(argv[1]);
        if(port) {
            try {
                boost::asio::io_service io_service;
                TcpServer server(io_service, port.value());
                LOG_NO_CONSOLE("SYS", "Server is now running. Access port: " << port.value());
                io_service.run();
                ret_code = EXIT_SUCCESS;
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        } else {
            LOG("SYS", "Wrong port number format!");
        }
    } else {
        LOG("SYS", "Wrong number of arguments (You forgot to indicate port value)");
    }
    return ret_code;
}