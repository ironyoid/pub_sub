#include "tcp_client.hpp"
#include "keyboard.hpp"
#include "utils.hpp"
#include <_types/_uint16_t.h>

using namespace ErrorCodes;

TcpConnection::pointer TcpConnection::Create(boost::asio::io_service &io_service, const std::string &addr) {
    return pointer(new TcpConnection(io_service, addr));
}

TcpConnection::TcpConnection(boost::asio::io_service &io_service, const std::string &addr) :
    socket_(io_service),
    addr_(addr){};

eStatus_t TcpConnection::Connect(uint16_t port, const std::string &name) {
    name_ = name;
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
    std::cout << "ERROR: " << error << std::endl;
    if((boost::asio::error::eof != error) && (boost::asio::error::connection_reset != error)
       && (boost::asio::error::operation_aborted != error)) {
        std::string messageP;
        std::stringstream ss;
        ss << &message_;
        ss.flush();
        messageP = ss.str();

        std::cout << messageP;
        Start();
    } else {
        std::cout << name_ << " Connection error!" << std::endl;
    }
}

void TcpConnection::Disconnect(void) {
    socket_.close();
}

TcpConnection::~TcpConnection() {
    std::cout << name_ << " has been deleted!" << std::endl;
}

TcpClient::TcpClient(boost::asio::io_service &io_service, std::string &addr) : io_service_(io_service), addr_(addr) {
}

eStatus_t TcpClient::Connect(uint16_t port, const std::string &name) {
    eStatus_t ret = eStatus_GeneralError;
    TcpConnection::pointer ptr = TcpConnection::Create(io_service_, addr_);
    if(eStatus_Ok == ptr->Connect(port, name)) {
        std::string tmp = "CONNECT " + name + "\n";
        if(ptr->Write(tmp) == tmp.size()) {
            ptr->Start();
            weak_connection = ptr;
            ret = eStatus_Ok;
        }
    }
    return ret;
}
