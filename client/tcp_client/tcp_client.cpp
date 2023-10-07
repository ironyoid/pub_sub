#include "tcp_client.hpp"

using namespace ErrorCodes;

TcpClient::pointer TcpClient::Create(boost::asio::io_service &io_service, const std::string &addr) {
    return pointer(new TcpClient(io_service, addr));
}

TcpClient::TcpClient(boost::asio::io_service &io_service, const std::string &addr) : socket_(io_service), addr_(addr){};

eStatus_t TcpClient::Connect(uint16_t port, const std::string &name) {
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

size_t TcpClient::Write(const std::string &msg) {
    size_t ret_size = 0;
    ret_size = boost::asio::write(socket_,
                                  boost::asio::buffer(msg.c_str(), msg.size()),
                                  boost::asio::transfer_at_least(msg.size()));
    return ret_size;
}

void TcpClient::Start(void) {
    async_read_until(socket_,
                     message_,
                     '\n',
                     [self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                         self->HandleRead(error, bytes_transferred);
                     });
}

void TcpClient::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
    if((boost::asio::error::eof != error) && (boost::asio::error::connection_reset != error)) {
        std::string messageP;
        std::stringstream ss;
        ss << &message_;
        ss.flush();
        messageP = ss.str();

        std::cout << messageP;
        Start();
    } else {
    }
}

void TcpClient::Disconnect(void) {
    socket_.close();
}
