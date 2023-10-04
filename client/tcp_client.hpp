#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <atomic>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"

using namespace ErrorCodes;

class TcpClient
{
   public:
    TcpClient(std::string &addr) : socket_(ios_), addr_(addr){};

    eStatus_t Connect (uint16_t port, std::string &name) {
        eStatus_t ret = eStatus_GeneralError;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(addr_), port);
        try {
            socket_.connect(endpoint);
            ret = eStatus_Ok;
        } catch(std::exception &e) {
            std::cerr << "Exception: " << e.what() << "\n";
        }

        return ret;
    }

    size_t Write (std::string &msg) {
        size_t ret_size = 0;
        ret_size = boost::asio::write(socket_,
                                      boost::asio::buffer(msg.c_str(), msg.size()),
                                      boost::asio::transfer_at_least(msg.size()));
        return ret_size;
    }

    size_t Read (std::string &msg) {
        size_t ret_size = 0;
        boost::asio::read_until(socket_, boost::asio::dynamic_buffer(msg), '\n');
        ret_size = msg.size();
        return ret_size;
    }

    void Disconnect (void) {
        socket_.close();
    }

   private:
    std::string addr_;
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::socket socket_;
};