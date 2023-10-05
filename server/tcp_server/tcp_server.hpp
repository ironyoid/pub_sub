#ifndef _TCP_SERVER_H__
#define _TCP_SERVER_H__
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <array>
#include <map>
#include <list>
#include "utils.hpp"

using boost::asio::ip::tcp;
using namespace ErrorCodes;

class Broker;

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
   public:
    using pointer = boost::shared_ptr<TcpConnection>;

    static pointer Create (const boost::asio::any_io_executor &io_context, Broker &broker);
    tcp::socket &Socket ();
    void Start ();

   private:
    TcpConnection(const boost::asio::any_io_executor &io_context, Broker &broker);

    void HandleWrite (const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);

    Broker &broker_;
    tcp::socket socket_;
    boost::asio::streambuf message_;
    std::string name_;
};

class Broker
{
   public:
    using ElementType = TcpConnection::pointer;
    using ListType = std::list<ElementType>;
    using StorageType = std::map<std::string, ListType>;

    Broker();

    size_t GetNumberOfSubscribers (std::string &topic);
    eStatus_t Subscribe (std::string &topic, ElementType element);
    eStatus_t Unsubscribe (std::string &topic, ElementType element);

   private:
    StorageType storage_;
};

class TcpServer
{
   public:
    TcpServer(boost::asio::io_service &io_service, Broker &broker);

   private:
    void StartAccept ();
    void HandleAccept (TcpConnection::pointer new_connection, const boost::system::error_code &error);

    Broker &broker_;
    tcp::acceptor acceptor_;
};

#endif /* _TCP_SERVER_H__ */