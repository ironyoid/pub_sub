#include "tcp_client.hpp"
#include "console_input.hpp"
#include "utils.hpp"
#include <stdint.h>
#include "logs.hpp"
#include <boost/make_shared.hpp>

using namespace ErrorStatus;
namespace Network {

    TcpConnection::pointer TcpConnection::Create(boost::asio::io_service &io_service,
                                                 const std::string &addr,
                                                 const std::string &name,
                                                 std::function<void(const std::string &name)> on_close) {
        return boost::make_shared<TcpConnection>(Tag{}, io_service, addr, name, on_close);
    }

    TcpConnection::TcpConnection(Tag,
                                 boost::asio::io_service &io_service,
                                 const std::string &addr,
                                 const std::string &name,
                                 std::function<void(const std::string &name)> on_close) :
        name_(name),
        addr_(addr),
        socket_(io_service),
        on_close_(on_close){};

    eStatus_t TcpConnection::Connect(uint16_t port) {
        eStatus_t ret = eStatus_t::GeneralError;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(addr_), port);

        boost::system::error_code ec;
        socket_.connect(endpoint, ec);
        switch(ec.value()) {
            case boost::system::errc::success:
                ret = eStatus_t::Ok;
                break;
            case boost::system::errc::address_not_available:
                ret = eStatus_t::WrongPort;
                break;
            case boost::asio::error::already_connected:
                ret = eStatus_t::PortAlreadyInUse;
                break;
            case boost::asio::error::connection_refused:
                ret = eStatus_t::ConnectionRefused;
                break;
            default:
                break;
        }

        return ret;
    }

    size_t TcpConnection::Write(const std::string &msg) {
        size_t ret_size = 0;
        ret_size = boost::asio::write(socket_,
                                      boost::asio::buffer(msg.data(), msg.size()),
                                      boost::asio::transfer_at_least(msg.size()));
        return ret_size;
    }

    void TcpConnection::Start(void) {
        async_read_until(
            socket_,
            message_,
            '\n',
            [this, self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                HandleRead(error, bytes_transferred);
            });
    }

    void TcpConnection::HandleRead(const boost::system::error_code &error, size_t) {
        if(!error) {
            auto msg = Utils::StreamBufToString(message_);
            std::cout << "[Message] " << msg;
            Start();
        } else {
            LOG_ERASE("SYS", "[" << name_ << "] Connection error!");
            on_close_(name_);
            Disconnect();
        }
    }

    void TcpConnection::Disconnect(void) {
        try {
            socket_.close();
            LOG_ERASE("SYS", "[" << name_ << "] Socket has been closed!");
        } catch(boost::system::error_code &err) {
            LOG_ERASE("SYS", "[" << name_ << "] couldn't close ");
        }
    }

    TcpConnection::~TcpConnection() {
        LOG_ERASE("SYS", "[" << name_ << "] Has been closed!");
    }

    TcpClient::TcpClient(boost::asio::io_service &io_service, const std::string &addr) :
        io_service_(io_service),
        addr_(addr) {
    }

    eStatus_t TcpClient::Connect(uint16_t port, const std::string &name) {
        eStatus_t ret = eStatus_t::GeneralError;
        if(nullptr == tcp_connection.get()) {
            tcp_connection
                = TcpConnection::Create(io_service_, addr_, name, [this] (const std::string &name) { OnClose(name); });
            ret = tcp_connection->Connect(port);
            if(eStatus_t::Ok == ret) {
                const std::string tmp = "CONNECT " + name + "\n";
                if(tcp_connection->Write(tmp) == tmp.size()) {
                    tcp_connection->Start();
                    ret = eStatus_t::Ok;
                }
            }
        } else {
            ret = eStatus_t::GeneralError;
        }
        return ret;
    }

    eStatus_t TcpClient::Disconnect(void) {
        eStatus_t ret = eStatus_t::LostConnection;
        tcp_connection->Disconnect();
        ret = eStatus_t::Ok;
        return ret;
    }
    void TcpClient::OnClose(const std::string &name) {
        LOG_ERASE("SYS", "[" << name << "] It's time to close connection!");
        tcp_connection.reset();
    }

} // namespace Network