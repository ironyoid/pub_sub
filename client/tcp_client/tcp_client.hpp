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

class KeyBoardRoutine;

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
   public:
    using pointer = boost::shared_ptr<TcpConnection>;
    using weak_pointer = boost::weak_ptr<TcpConnection>;
    static pointer Create (boost::asio::io_service &io_service, const std::string &addr, KeyBoardRoutine &keyboard);
    eStatus_t Connect (uint16_t port, const std::string &name);
    size_t Write (const std::string &msg);
    void Start (void);
    void Disconnect (void);
    ~TcpConnection();

   private:
    explicit TcpConnection(boost::asio::io_service &io_service, const std::string &addr, KeyBoardRoutine &keyboard);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);
    /* Do not copy! */
    TcpConnection(const TcpConnection &) = delete;
    void operator=(const TcpConnection &) = delete;
    boost::asio::streambuf message_;
    std::string name_;
    std::string addr_;
    boost::asio::ip::tcp::socket socket_;
    KeyBoardRoutine &keyboard_;
};

class TcpClient
{
   public:
    TcpClient(boost::asio::io_service &io_service, std::string &addr);
    TcpConnection::pointer Start (KeyBoardRoutine &keyboard);

   private:
    boost::asio::io_service &io_service_;
    std::string &addr_;
};

#endif /* _TCP_CLIENT_H__ */