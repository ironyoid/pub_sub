#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <array>
#include "tcp_server.hpp"

int main (int argc, char *argv[]) {
    try {
        Broker broker{};
        boost::asio::io_service io_service;
        TcpServer server(io_service, broker);
        io_service.run();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}