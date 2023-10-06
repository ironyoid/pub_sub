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
#include <set>
#include <list>
#include <vector>
#include "utils.hpp"
#include "parser.hpp"
#include "commands.hpp"

using boost::asio::ip::tcp;
using namespace ErrorCodes;
using namespace Commands;

class Broker;
struct ContextContainer;

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
   public:
    using pointer = boost::shared_ptr<TcpConnection>;

    static pointer Create (const boost::asio::any_io_executor &io_context,
                           Broker &broker,
                           CommandDispatcher<ContextContainer> &parser);
    tcp::socket &Socket ();
    void Start ();
    void Print (void);
    void SendMessage (std::string &data);
    ~TcpConnection();

    std::string name;

   private:
    TcpConnection(const boost::asio::any_io_executor &io_context,
                  Broker &broker,
                  CommandDispatcher<ContextContainer> &parser);
    /* Do not copy! */
    TcpConnection(const TcpConnection &) = delete;
    void operator=(const TcpConnection &) = delete;

    void HandleWrite (const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);

    Broker &broker_;
    std::set<std::string> topics_;
    CommandDispatcher<ContextContainer> &parser_;
    tcp::socket socket_;
    boost::asio::streambuf message_;

    friend class Commands::Connect<ContextContainer>;
    friend class Commands::Publish<ContextContainer>;
    friend class Commands::Subscribe<ContextContainer>;
    friend class Commands::Unsubscribe<ContextContainer>;
};

class Broker
{
   public:
    using ElementType = TcpConnection::pointer;
    using ListType = std::set<ElementType>;
    using StorageType = std::map<std::string, ListType>;

    Broker();

    size_t GetNumberOfSubscribers (std::string &topic);
    eStatus_t Subscribe (std::string &topic, ElementType element);
    eStatus_t Unsubscribe (std::string &topic, ElementType element);
    eStatus_t Notify (std::string &topic, std::string &data);
    void Print (void);

   private:
    /* Do not copy! */
    Broker(const Broker &) = delete;
    void operator=(const Broker &) = delete;
    StorageType storage_;
};

struct ContextContainer {
    ContextContainer(Broker &broker, TcpConnection::pointer ptr) : broker(broker), ptr(ptr) {
    }
    TcpConnection::pointer ptr;
    Broker &broker;
};
class TcpServer
{
   public:
    TcpServer(boost::asio::io_service &io_service, uint16_t port);

   private:
    void StartAccept ();
    void HandleAccept (TcpConnection::pointer new_connection, const boost::system::error_code &error);
    /* Do not copy! */
    TcpServer(const TcpServer &) = delete;
    void operator=(const TcpServer &) = delete;

    Broker broker_;
    CommandDispatcher<ContextContainer> parser_;
    tcp::acceptor acceptor_;

    Commands::Connect<ContextContainer> name_cmd;
    Commands::Publish<ContextContainer> pub_cmd;
    Commands::Subscribe<ContextContainer> sub_cmd;
    Commands::Unsubscribe<ContextContainer> unsub_cmd;
};

#endif /* _TCP_SERVER_H__ */