#include "tcp_server.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include <iostream>

using std::cout;
using std::endl;
using namespace ErrorCodes;
using namespace Commands;

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

eStatus_t Broker::Subscribe(const std::string &topic, ElementType element) {
    eStatus_t ret = eStatus_GeneralError;
    bool is_exist = false;
    ListType::const_iterator set_itr = storage_[topic].find(element);
    if(set_itr == storage_[topic].end()) {
        storage_[topic].insert(element);
        ret = eStatus_Ok;
    } else {
        ret = eStatus_ElementExistsError;
    }
    return ret;
}

eStatus_t Broker::Unsubscribe(const std::string &topic, ElementType element) {
    eStatus_t ret = eStatus_GeneralError;
    StorageType::const_iterator map_itr = storage_.find(topic);
    if(map_itr != storage_.end()) {
        ListType::const_iterator set_itr = storage_[topic].find(element);
        if(set_itr != storage_[topic].end()) {
            storage_[topic].erase(set_itr);
            if(0 == storage_[topic].size()) {
                storage_.erase(topic);
            }
            ret = eStatus_Ok;
        } else {
            ret = eStatus_ElementExistsError;
        }
    }
    return ret;
}

eStatus_t Broker::Notify(const std::string &topic, const std::string &data) {
    eStatus_t ret = eStatus_GeneralError;
    StorageType::const_iterator map_itr = storage_.find(topic);
    if(map_itr != storage_.end()) {
        for(const auto &n : storage_[topic]) {
            cout << "send msg to: " << n->name << endl;
            n->SendMessage("Topic: " + topic + " Data: " + data);
        }
        ret = eStatus_Ok;
    }
    return ret;
}

void Broker::Print(void) {
    cout << endl << "[BROKER MAP size: " << storage_.size() << "]" << endl;
    std::set<ElementType>::iterator itr;
    for(auto n : storage_) {
        cout << "[" << n.first << " ]: ";
        std::string tmp{ "" };
        for(itr = n.second.begin(); itr != n.second.end(); itr++) {
            tmp = tmp + (*itr)->name + ", ";
        }
        tmp.pop_back();
        tmp.pop_back();
        cout << tmp << endl;
    }
    cout << endl;
}

TcpConnection::pointer TcpConnection::Create(const boost::asio::any_io_executor &io_service,
                                             Broker &broker,
                                             CommandDispatcher<ContextContainer> &parser) {
    return pointer(new TcpConnection(io_service, broker, parser));
}

tcp::socket &TcpConnection::Socket() {
    return socket_;
}

TcpConnection::~TcpConnection() {
    for(auto n : topics_) {
        broker_.Unsubscribe(n, shared_from_this());
    }
    std::cout << name << " has been deleted!" << std::endl;
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

TcpConnection::TcpConnection(const boost::asio::any_io_executor &io_service,
                             Broker &broker,
                             CommandDispatcher<ContextContainer> &parser) :
    socket_(io_service),
    broker_(broker),
    parser_(parser),
    topics_(),
    name() {
}

void TcpConnection::HandleWrite(const boost::system::error_code &error, size_t bytes_transferred) {
}

void TcpConnection::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
    if((boost::asio::error::eof != error) && (boost::asio::error::connection_reset != error)) {
        std::string messageP;
        std::stringstream ss;
        ss << &message_;
        ss.flush();
        messageP = ss.str();

        ContextContainer context{ broker_, shared_from_this() };
        ErrorCodes::eStatus_t ret = parser_.ParseRawString(messageP, context);
        Start();

    } else {
        std::cout << "[" << name << "] has been disconnected!" << std::endl;
    }
}

void TcpConnection::Print(void) {
    std::cout << "[TOPICS][" << name << "]: ";
    for(auto n : topics_) {
        std::cout << n << ", ";
    }
    std::cout << std::endl;
}

void TcpConnection::SendMessage(const std::string &data) {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(data),
                             boost::bind(&TcpConnection::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

TcpServer::TcpServer(boost::asio::io_service &io_service, uint16_t port) :
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    broker_(),
    parser_(),
    name_cmd(),
    pub_cmd(),
    sub_cmd(),
    unsub_cmd() {
    parser_.AddCommand("CONNECT", name_cmd);
    parser_.AddCommand("PUBLISH", pub_cmd);
    parser_.AddCommand("SUBSCRIBE", sub_cmd);
    parser_.AddCommand("UNSUBSCRIBE", unsub_cmd);
    StartAccept();
}

void TcpServer::StartAccept() {
    TcpConnection::pointer new_connection = TcpConnection::Create(acceptor_.get_executor(), broker_, parser_);

    acceptor_.async_accept(
        new_connection->Socket(),
        boost::bind(&TcpServer::HandleAccept, this, new_connection, boost::asio::placeholders::error));
}

void TcpServer::HandleAccept(TcpConnection::pointer new_connection, const boost::system::error_code &error) {
    if(!error) {
        new_connection->Start();
    }

    StartAccept();
}
