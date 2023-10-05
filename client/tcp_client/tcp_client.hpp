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
    TcpClient(std::string &addr);
    eStatus_t Connect (uint16_t port, std::string &name);
    size_t Write (std::string &msg);
    size_t Read (std::string &msg);
    void Disconnect (void);

   private:
    std::string addr_;
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::socket socket_;
};