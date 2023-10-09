#include "tcp_client.hpp"
#include "console_input.hpp"
#include "utils.hpp"
#include <_types/_uint16_t.h>
#include "logs.hpp"

namespace Network {

    TcpConnection::pointer TcpConnection::Create(boost::asio::io_service &io_service,
                                                 const std::string &addr,
                                                 const std::string &name) {
        return pointer(new TcpConnection(io_service, addr, name));
    }

    TcpConnection::TcpConnection(boost::asio::io_service &io_service,
                                 const std::string &addr,
                                 const std::string &name) :
        socket_(io_service),
        addr_(addr),
        name_(name){};

    ErrorCodes::eStatus_t TcpConnection::Connect(uint16_t port) {
        ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(addr_), port);

        boost::system::error_code ec;
        socket_.connect(endpoint, ec);
        switch(ec.value()) {
            case boost::system::errc::success:
                ret = ErrorCodes::eStatus_Ok;
                break;
            case boost::system::errc::address_not_available:
                ret = ErrorCodes::eStatus_WrongPort;
                break;
            case boost::asio::error::already_connected:
                ret = ErrorCodes::eStatus_PortAlreadyInUse;
                break;
            case boost::asio::error::connection_refused:
                ret = ErrorCodes::eStatus_ConnectionRefused;
                break;
            default:
                break;
        }

        return ret;
    }

    size_t TcpConnection::Write(const std::string &msg) {
        size_t ret_size = 0;
        ret_size = boost::asio::write(socket_,
                                      boost::asio::buffer(msg.c_str(), msg.size()),
                                      boost::asio::transfer_at_least(msg.size()));
        return ret_size;
    }

    void TcpConnection::Start(void) {
        async_read_until(
            socket_,
            message_,
            '\n',
            [self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                self->HandleRead(error, bytes_transferred);
            });
    }

    void TcpConnection::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
        if((boost::asio::error::eof != error) && (boost::asio::error::connection_reset != error)
           && (boost::asio::error::operation_aborted != error)) {
            std::string messageP;
            std::stringstream ss;
            ss << &message_;
            ss.flush();
            messageP = ss.str();
            messageP.pop_back();
            LOG_ERASE("Message", messageP);

            Start();
        } else {
            LOG_ERASE("SYS", name_ << " Connection error!");
        }
    }

    void TcpConnection::Disconnect(void) {
        socket_.close();
    }

    TcpConnection::~TcpConnection() {
        LOG_ERASE("SYS", "[" << name_ << "] Has been closed!");
    }

    TcpClient::TcpClient(boost::asio::io_service &io_service, const std::string &addr) :
        io_service_(io_service),
        addr_(addr),
        is_connected(false) {
    }

    ErrorCodes::eStatus_t TcpClient::Connect(uint16_t port, const std::string &name) {
        ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
        if(true == weak_connection.expired()) {
            TcpConnection::pointer ptr = TcpConnection::Create(io_service_, addr_, name);
            ret = ptr->Connect(port);
            if(ErrorCodes::eStatus_Ok == ret) {
                std::string tmp = "CONNECT " + name + "\n";
                if(ptr->Write(tmp) == tmp.size()) {
                    ptr->Start();
                    weak_connection = ptr;
                    is_connected = true;
                    ret = ErrorCodes::eStatus_Ok;
                }
            }
        }
        return ret;
    }

    ErrorCodes::eStatus_t TcpClient::Disconnect(void) {
        ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_LostConnection;
        if(false == weak_connection.expired()) {
            auto shrd_tmp = weak_connection.lock();
            shrd_tmp->Disconnect();
            ret = ErrorCodes::eStatus_Ok;
            is_connected = false;
        }
        return ret;
    }
} // namespace Network