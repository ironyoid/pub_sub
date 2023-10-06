#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <atomic>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"
#include <boost/bind.hpp>

using namespace ErrorCodes;

class TcpClient
{
   public:
    explicit TcpClient(std::string &addr);
    eStatus_t Connect (uint16_t port, std::string &name);
    size_t Write (std::string &msg);
    void StartRead (void);
    void Disconnect (void);

    boost::asio::io_service io_context;

   private:
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);

    boost::asio::streambuf message_;
    std::string addr_;
    boost::asio::ip::tcp::socket socket_;
};