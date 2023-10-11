#include "tcp_server.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include <iostream>
#include <boost/make_shared.hpp>

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using namespace ErrorStatus;
using namespace Commands;
using std::unique_ptr;
namespace Network {
    Network::Broker::Broker() {
    }

    eStatus_t Broker::Subscribe(const std::string &topic, ElementType element) {
        auto [_, is_found] = storage_[topic].emplace(element);
        return is_found ? eStatus_t::Ok : eStatus_t::ElementExistsError;
    }

    eStatus_t Broker::Unsubscribe(const std::string &topic, ElementType element) {
        eStatus_t ret = eStatus_t::GeneralError;
        auto map_itr = storage_.find(topic);
        if(map_itr != storage_.end()) {
            auto &elem = map_itr->second;
            auto set_itr = elem.find(element);
            if(set_itr != elem.end()) {
                elem.erase(set_itr);
                if(0 == elem.size()) {
                    storage_.erase(topic);
                }
                ret = eStatus_t::Ok;
            } else {
                ret = eStatus_t::ElementExistsError;
            }
        }
        return ret;
    }

    eStatus_t Broker::Notify(const std::string &topic, const std::string &data) {
        eStatus_t ret = eStatus_t::GeneralError;
        auto map_itr = storage_.find(topic);
        if(map_itr != storage_.end()) {
            for(const auto &n : map_itr->second) {
                LOG_NO_INPUT("SYS", "Send message: [" << data << "] to the topic: [" << n->name << "]");
                n->AddToQueue("Topic: " + topic + " Data: " + data + "\n");
            }
            ret = eStatus_t::Ok;
        }
        return ret;
    }

    void Broker::Print(void) {
        cout << "[BROKER MAP size: " << storage_.size() << "]" << endl;
        std::set<ElementType>::iterator itr;
        for(const auto &n : storage_) {
            cout << "[" << n.first << "]: ";
            std::string tmp{ "" };
            for(itr = n.second.begin(); itr != n.second.end(); itr++) {
                tmp = tmp + (*itr)->name + ", ";
            }
            try {
                tmp.resize(tmp.size() - 2);
            } catch(const std::length_error &e) {
                cout << "Corrupted" << endl;
            }
            cout << tmp << endl;
        }
        cout << endl;
    }

    TcpConnection::pointer TcpConnection::Create(const boost::asio::any_io_executor &io_service,
                                                 Broker &broker,
                                                 Parser::CommandDispatcher<ContextContainer> &parser) {
        return boost::make_shared<TcpConnection>(Tag{}, io_service, broker, parser);
    }

    TcpConnection::TcpConnection(Tag,
                                 const boost::asio::any_io_executor &io_service,
                                 Broker &broker,
                                 Parser::CommandDispatcher<ContextContainer> &parser) :
        broker_(broker),
        parser_(parser),
        socket_(io_service) {
    }

    tcp::socket &TcpConnection::Socket() {
        return socket_;
    }

    void TcpConnection::Start() {
        async_read_until(
            socket_,
            message_,
            '\n',
            [this, self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                HandleRead(error, bytes_transferred);
            });
    }

    void TcpConnection::AddToQueue(const std::string &data) {
        accumulator.append(data);
        if(!accumulator.empty()) {
            if(for_send.empty()) {
                for_send.swap(accumulator);
                boost::asio::async_write(socket_,
                                         boost::asio::buffer(for_send),
                                         [this, self = shared_from_this()] (const boost::system::error_code &error,
                                                                            size_t bytes_transferred) {
                                             HandleWrite(error, bytes_transferred);
                                         });
            }
        }
    }

    void TcpConnection::HandleWrite(const boost::system::error_code &error, size_t) {
        if(!error) {
            for_send.clear();
            if(!accumulator.empty()) {
                AddToQueue({});
            }
        } else {
            for_send.clear();
            accumulator.clear();
            Disconnect();
            LOG_NO_INPUT("SYS", "[" << name << "] Write error! Code: " << error.to_string());
        }
    }

    void TcpConnection::HandleRead(const boost::system::error_code &error, size_t) {
        if(!error) {
            auto msg = Utils::StreamBufToString(message_);
            ContextContainer context{ broker_, shared_from_this() };
            parser_.ParseRawString(msg, context);
            Start();

        } else {
            Disconnect();
            LOG_NO_INPUT("SYS", "[" << name << "] has been disconnected!");
            broker_.Print();
        }
    }

    void TcpConnection::UnsubscribeFromAll(void) {
        for(const auto &n : topics_) {
            broker_.Unsubscribe(n, shared_from_this());
        }
        topics_.clear();
    }

    void TcpConnection::Disconnect(void) {
        UnsubscribeFromAll();
        try {
            socket_.close();
            LOG_NO_INPUT("SYS", "[" << name << "] socket has been closed!");
        } catch(boost::system::error_code &err) {
            LOG_NO_INPUT("SYS", "[" << name << "] couldn't close the socket");
        }
    }

    void TcpConnection::Print(void) {
        std::cout << "[TOPICS][" << name << "]: ";
        for(const auto &n : topics_) {
            std::cout << n << ", ";
        }
        std::cout << std::endl;
    }

    TcpServer::TcpServer(boost::asio::io_service &io_service, uint16_t port) :
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
        unique_ptr<Connect<ContextContainer>> connect = std::make_unique<Connect<ContextContainer>>();
        unique_ptr<Publish<ContextContainer>> publish = std::make_unique<Publish<ContextContainer>>();
        unique_ptr<Subscribe<ContextContainer>> subscribe = std::make_unique<Subscribe<ContextContainer>>();
        unique_ptr<Unsubscribe<ContextContainer>> unsubscribe = std::make_unique<Unsubscribe<ContextContainer>>();
        parser_.AddCommand(std::move(connect));
        parser_.AddCommand(std::move(publish));
        parser_.AddCommand(std::move(subscribe));
        parser_.AddCommand(std::move(unsubscribe));
        StartAccept();
    }

    void TcpServer::StartAccept() {
        TcpConnection::pointer new_connection = TcpConnection::Create(acceptor_.get_executor(), broker_, parser_);

        acceptor_.async_accept(
            new_connection->Socket(),
            [this, new_connection] (const boost::system::error_code error) { HandleAccept(new_connection, error); });
    }

    void TcpServer::HandleAccept(TcpConnection::pointer new_connection, const boost::system::error_code &error) {
        if(!error) {
            new_connection->Start();
        }

        StartAccept();
    }

    TcpConnection::~TcpConnection() {
        LOG_NO_INPUT("SYS",
                     "[" << name << "]"
                         << " has been deleted!");
    }
} // namespace Network