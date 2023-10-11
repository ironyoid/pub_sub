#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include "tcp_server.hpp"
#include "logs.hpp"
#include "utils.hpp"

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;
    if(2 == argc) {
        const auto port = Utils::GetPortFromStr(argv[1]);
        if(port) {
            try {
                boost::asio::io_service io_service;
                Network::TcpServer server(io_service, port.value());
                LOG_NO_INPUT("SYS", "Server is now running. Access port: " << port.value());
                io_service.run();
                ret_code = EXIT_SUCCESS;
            } catch(const std::exception &e) {
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