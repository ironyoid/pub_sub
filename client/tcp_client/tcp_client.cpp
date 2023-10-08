#include "tcp_client.hpp"
#include "console_input.hpp"
#include "utils.hpp"
#include <_types/_uint16_t.h>

using namespace ErrorCodes;

TcpConnection::pointer TcpConnection::Create(boost::asio::io_service &io_service,
                                             const std::string &addr,
                                             const std::string &name) {
    return pointer(new TcpConnection(io_service, addr, name));
}

TcpConnection::TcpConnection(boost::asio::io_service &io_service, const std::string &addr, const std::string &name) :
    socket_(io_service),
    addr_(addr),
    name_(name){};

eStatus_t TcpConnection::Connect(uint16_t port) {
    eStatus_t ret = eStatus_GeneralError;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(addr_), port);
    try {
        socket_.connect(endpoint);
        ret = eStatus_Ok;
    } catch(std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
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
    async_read_until(socket_,
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
        std::cout << "[Message] " << messageP;

        Start();
    } else {
        std::cout << name_ << " Connection error!" << std::endl;
    }
}

void TcpConnection::Disconnect(void) {
    socket_.close();
}

TcpConnection::~TcpConnection() {
    std::cout << "[" << name_ << "] Has been closed!" << std::endl;
}

TcpClient::TcpClient(boost::asio::io_service &io_service, const std::string &addr) :
    io_service_(io_service),
    addr_(addr),
    is_connected(false) {
}

eStatus_t TcpClient::Connect(uint16_t port, const std::string &name) {
    eStatus_t ret = eStatus_GeneralError;
    if(false == is_connected) {
        TcpConnection::pointer ptr = TcpConnection::Create(io_service_, addr_, name);
        if(eStatus_Ok == ptr->Connect(port)) {
            std::string tmp = "CONNECT " + name + "\n";
            if(ptr->Write(tmp) == tmp.size()) {
                ptr->Start();
                weak_connection = ptr;
                is_connected = true;
                ret = eStatus_Ok;
            }
        }
    }
    return ret;
}

eStatus_t TcpClient::Disconnect(void) {
    eStatus_t ret = eStatus_LostConnection;
    if(false == weak_connection.expired()) {
        auto shrd_tmp = weak_connection.lock();
        shrd_tmp->Disconnect();
        ret = eStatus_Ok;
        is_connected = false;
    }
    return ret;
}