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
#include <boost/enable_shared_from_this.hpp>

using namespace ErrorCodes;

class TcpClient : public boost::enable_shared_from_this<TcpClient>
{
   public:
    using pointer = boost::shared_ptr<TcpClient>;
    static pointer Create (const std::string &addr);
    eStatus_t Connect (uint16_t port, std::string &name);

    size_t Write (std::string &msg);
    void StartRead (void);
    void Disconnect (void);

    boost::asio::io_service io_context;
    boost::asio::ip::tcp::socket socket_;

   private:
    explicit TcpClient(const std::string &addr);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);
    /* Do not copy! */
    TcpClient(const TcpClient &) = delete;
    void operator=(const TcpClient &) = delete;
    boost::asio::streambuf message_;
    std::string addr_;
};