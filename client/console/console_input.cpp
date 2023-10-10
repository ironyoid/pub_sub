#include "console_input.hpp"
#include "parser.hpp"
#include "tcp_client.hpp"
#include "utils.hpp"
#include <iostream>
#include <logs.hpp>
#include <memory>
#include <boost/make_shared.hpp>

using namespace ErrorStatus;
namespace ConsoleIO {

    ConsoleInput::pointer ConsoleInput::Create(boost::asio::io_service &io_service,
                                               Network::TcpClient client,
                                               Parser::CommandDispatcher<Network::TcpClient> dispatcher) {
        return boost::make_shared<ConsoleInput>(Tag{}, io_service, std::move(client), std::move(dispatcher));
    }

    void ConsoleInput::Start(void) {
        async_read_until(
            input_,
            message_,
            '\n',
            [this, self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                HandleRead(error, bytes_transferred);
            });
    }

    ConsoleInput::ConsoleInput(Tag,
                               boost::asio::io_service &io_service,
                               Network::TcpClient client,
                               Parser::CommandDispatcher<Network::TcpClient> dispatcher) :
        dispatcher_(std::move(dispatcher)),
        client_(std::move(client)),
        input_(io_service, ::dup(STDIN_FILENO)) {
    }

    void ConsoleInput::HandleRead(const boost::system::error_code &error, size_t) {
        if(!error) {
            auto msg = Utils::StreamBufToString(message_);

            eStatus_t status = dispatcher_.ParseRawString(msg, client_);
            switch(status) {
                case eStatus_t::Ok:
                    LOG("SYS", "Comand has been executed successfully!");
                    break;
                case eStatus_t::WrongArgsNum:
                    LOG("SYS", "Wrong number of command parametrs!");
                    break;
                case eStatus_t::GeneralError:
                    LOG("SYS", "Unknown error!");
                    break;
                case eStatus_t::LostConnection:
                    LOG("SYS", "There are no any connections!");
                    break;
                case eStatus_t::WrongPort:
                    LOG("SYS", "Port argument is invalid!");
                    break;
                case eStatus_t::PortAlreadyInUse:
                    LOG("SYS", "Port is already in use!");
                    break;
                case eStatus_t::ConnectionRefused:
                    LOG("SYS", "Connection refused!");
                    break;

                default:
                    break;
            }

            Start();
        } else {
            LOG("SYS", "We have just lost console!");
        }
    }
} // namespace ConsoleIO