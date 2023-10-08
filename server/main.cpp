#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <array>
#include "tcp_server.hpp"
#include "logs.hpp"
namespace {
    const uint16_t SERVER_PORT = 1936;
}

int main (int argc, char *argv[]) {
    try {
        boost::asio::io_service io_service;
        TcpServer server(io_service, SERVER_PORT);
        LOG_NO_CONSOLE("SYS", "Server is now running. Access port: " << SERVER_PORT);
        io_service.run();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}