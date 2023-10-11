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
        input_(io_service, ::dup(STDIN_FILENO)),
        dispatcher_(std::move(dispatcher)),
        client_(std::move(client)) {
    }

    void ConsoleInput::HandleRead(const boost::system::error_code &error, size_t) {
        if(!error) {
            auto msg = Utils::StreamBufToString(message_);

            eStatus_t status = dispatcher_.ParseRawString(msg, client_);
            LOG_ERASE("SYS", ToString(status));
            Start();
        } else {
            LOG_ERASE("SYS", "We have just lost console!");
        }
    }
} // namespace ConsoleIO