#include "tcp_server.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include <iostream>

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using namespace ErrorCodes;
using namespace Commands;

namespace Network {
    Network::Broker::Broker() : storage_() {
    }

    eStatus_t Broker::Subscribe(const std::string &topic, ElementType element) {
        auto [_, is_found] = storage_[topic].emplace(element);
        return is_found ? eStatus_Ok : eStatus_ElementExistsError;
    }

    eStatus_t Broker::Unsubscribe(const std::string &topic, ElementType element) {
        eStatus_t ret = eStatus_GeneralError;
        auto map_itr = storage_.find(topic);
        if(map_itr != storage_.end()) {
            auto set_itr = storage_[topic].find(element);
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
        auto map_itr = storage_.find(topic);
        if(map_itr != storage_.end()) {
            for(const auto &n : storage_[topic]) {
                LOG_NO_INPUT("SYS", "Send message: [" << data << "] to the topic: [" << n->name << "]");

                n->AddToQueue("Topic: " + topic + " Data: " + data + "\n");
            }
            ret = eStatus_Ok;
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
            tmp.pop_back();
            tmp.pop_back();
            cout << tmp << endl;
        }
        cout << endl;
    }

    TcpConnection::pointer TcpConnection::Create(const boost::asio::any_io_executor &io_service,
                                                 Broker &broker,
                                                 Parser::CommandDispatcher<ContextContainer> &parser) {
        return pointer(new TcpConnection(io_service, broker, parser));
    }

    tcp::socket &TcpConnection::Socket() {
        return socket_;
    }

    TcpConnection::~TcpConnection() {
        LOG_NO_INPUT("SYS",
                     "[" << name << "]"
                         << " has been deleted!");
        for(const auto &n : topics_) {
            broker_.Unsubscribe(n, shared_from_this());
        }
    }

    void TcpConnection::Start() {
        async_read_until(
            socket_,
            message_,
            '\n',
            [self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                self->HandleRead(error, bytes_transferred);
            });
    }

    TcpConnection::TcpConnection(const boost::asio::any_io_executor &io_service,
                                 Broker &broker,
                                 Parser::CommandDispatcher<ContextContainer> &parser) :
        socket_(io_service),
        broker_(broker),
        parser_(parser) {
    }

    void TcpConnection::HandleWrite(const boost::system::error_code &error, size_t bytes_transferred) {
        /* TODO: Shall we handle errors here? */
        for_send.clear();
        if(!accumulator.empty()) {
            AddToQueue({});
        }
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
            for(const auto &n : topics_) {
                broker_.Unsubscribe(n, shared_from_this());
            }
            topics_.clear();
            LOG_NO_INPUT("SYS", "[" << name << "] has been disconnected!");
        }
    }

    void TcpConnection::Print(void) {
        std::cout << "[TOPICS][" << name << "]: ";
        for(const auto &n : topics_) {
            std::cout << n << ", ";
        }
        std::cout << std::endl;
    }

    void TcpConnection::AddToQueue(const std::string &data) {
        accumulator.append(data);
        if(for_send.empty()) {
            for_send.swap(accumulator);
            SendMessage(for_send);
        }
    }

    void TcpConnection::SendMessage(const std::string &data) {
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(data),
            [this, self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                HandleWrite(error, bytes_transferred);
            });
    }

    TcpServer::TcpServer(boost::asio::io_service &io_service, uint16_t port) :
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
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
            [this, new_connection] (const boost::system::error_code error) { HandleAccept(new_connection, error); });
    }

    void TcpServer::HandleAccept(TcpConnection::pointer new_connection, const boost::system::error_code &error) {
        if(!error) {
            new_connection->Start();
        }

        StartAccept();
    }
} // namespace Network