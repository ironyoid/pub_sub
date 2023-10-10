#ifndef _TCP_CLIENT_H__
#define _TCP_CLIENT_H__
#include <boost/array.hpp>
#include <atomic>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace Network {
    class ConsoleInput;
    class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
    {
        struct Tag {
        };

       public:
        using pointer = boost::shared_ptr<TcpConnection>;
        using weak_pointer = boost::weak_ptr<TcpConnection>;
        static pointer Create (boost::asio::io_service &io_service,
                               const std::string &addr,
                               const std::string &name,
                               std::function<void(const std::string &name)> on_close);
        ErrorStatus::eStatus_t Connect (uint16_t port);
        size_t Write (const std::string &msg);
        void Start (void);
        void Disconnect (void);
        TcpConnection(Tag,
                      boost::asio::io_service &io_service,
                      const std::string &addr,
                      const std::string &name,
                      std::function<void(const std::string &name)> OnClose);
        ~TcpConnection();

       private:
        void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);
        /* Do not copy! */
        TcpConnection(const TcpConnection &) = delete;
        void operator=(const TcpConnection &) = delete;

        boost::asio::streambuf message_;
        std::string name_;
        std::string addr_;
        boost::asio::ip::tcp::socket socket_;
        std::function<void(const std::string &name)> on_close_;
    };

    class TcpClient
    {
       public:
        TcpClient(boost::asio::io_service &io_service, const std::string &addr);
        TcpClient(TcpClient &&moved) noexcept : io_service_(moved.io_service_) {
            addr_ = std::move(moved.addr_);
            tcp_connection = std::move(moved.tcp_connection);
        }
        ErrorStatus::eStatus_t Connect (uint16_t port, const std::string &name);
        ErrorStatus::eStatus_t Disconnect (void);
        void OnClose (const std::string &name);
        TcpConnection::pointer tcp_connection;

       private:
        /* Do not copy! */
        TcpClient(const TcpClient &) = delete;
        void operator=(const TcpClient &) = delete;

        boost::asio::io_service &io_service_;
        std::string addr_;
    };
} // namespace Network
#endif /* _TCP_CLIENT_H__ */