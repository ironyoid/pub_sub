#include "tcp_server.hpp"

Broker::Broker() : storage_() {
}

size_t Broker::GetNumberOfSubscribers(std::string &topic) {
    size_t ret = 0;
    StorageType::const_iterator cmd_pair = storage_.find(topic);
    if(cmd_pair != storage_.end()) {
        ret = storage_[topic].size();
    }
    return ret;
}

eStatus_t Broker::Subscribe(std::string &topic, ElementType element) {
    eStatus_t ret = eStatus_GeneralError;
    storage_[topic].push_back(element);
    ret = eStatus_Ok;
    return ret;
}

eStatus_t Broker::Unsubscribe(std::string &topic, ElementType element) {
    eStatus_t ret = eStatus_GeneralError;
    StorageType::const_iterator cmd_pair = storage_.find(topic);
    if(cmd_pair != storage_.end()) {
        for(auto itr = storage_[topic].begin(); itr != storage_[topic].end(); itr++) {
            if(itr->get() == element.get()) {
                storage_[topic].erase(itr);
                ret = eStatus_Ok;
                break;
            }
        }
    }
    return ret;
}

TcpConnection::pointer TcpConnection::Create(const boost::asio::any_io_executor &io_context, Broker &broker) {
    return pointer(new TcpConnection(io_context, broker));
}

tcp::socket &TcpConnection::Socket() {
    return socket_;
}

void TcpConnection::Start() {
    async_read_until(socket_,
                     message_,
                     '\n',
                     boost::bind(&TcpConnection::HandleRead,
                                 shared_from_this(),
                                 boost::asio::placeholders::error,
                                 boost::asio::placeholders::bytes_transferred));
}

TcpConnection::TcpConnection(const boost::asio::any_io_executor &io_context, Broker &broker) :
    socket_(io_context),
    broker_(broker),
    name_() {
}

void TcpConnection::HandleWrite(const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/) {
}

void TcpConnection::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
    if((boost::asio::error::eof != error) && (boost::asio::error::connection_reset != error)) {
        std::string messageP;
        {
            std::stringstream ss;
            ss << &message_;
            ss.flush();
            messageP = ss.str();
        }
        //broker_.Subscribe(std::string & topic, ElementType element)

        std::cout << messageP;
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(messageP),
                                 boost::bind(&TcpConnection::HandleWrite,
                                             shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
        Start();
    } else {
        std::cout << "Error: Client has disconnected" << std::endl;
    }
}

TcpServer::TcpServer(boost::asio::io_service &io_service, Broker &broker) :
    acceptor_(io_service, tcp::endpoint(tcp::v4(), 1936)),
    broker_(broker) {
    StartAccept();
}

void TcpServer::StartAccept() {
    TcpConnection::pointer new_connection = TcpConnection::Create(acceptor_.get_executor(), broker_);

    acceptor_.async_accept(
        new_connection->Socket(),
        boost::bind(&TcpServer::HandleAccept, this, new_connection, boost::asio::placeholders::error));
}

void TcpServer::HandleAccept(TcpConnection::pointer new_connection, const boost::system::error_code &error) {
    if(!error) {
        std::cout << "A client connected" << std::endl;
        new_connection->Start();
    }

    StartAccept();
}
