#ifndef _TCP_CLIENT_H__
#define _TCP_CLIENT_H__
#include <boost/array.hpp>
#include <atomic>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using namespace ErrorCodes;

class TcpClient : public boost::enable_shared_from_this<TcpClient>
{
   public:
    using pointer = boost::shared_ptr<TcpClient>;
    static pointer Create (boost::asio::io_service &io_service, const std::string &addr);
    eStatus_t Connect (uint16_t port, const std::string &name);

    size_t Write (const std::string &msg);
    void Start (void);
    void Disconnect (void);

   private:
    explicit TcpClient(boost::asio::io_service &io_service, const std::string &addr);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);
    /* Do not copy! */
    TcpClient(const TcpClient &) = delete;
    void operator=(const TcpClient &) = delete;
    boost::asio::streambuf message_;
    std::string addr_;
    boost::asio::ip::tcp::socket socket_;
};

#endif /* _TCP_CLIENT_H__ */