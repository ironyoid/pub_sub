#include "console_input.hpp"
#include "parser.hpp"
#include "tcp_client.hpp"
#include "utils.hpp"
#include <iostream>
using std::cout;
using std::endl;

ConsoleInput::pointer ConsoleInput::Create(boost::asio::io_service &io_service,
                                           TcpClient client,
                                           CommandDispatcher<TcpClient> dispatcher) {
    return pointer(new ConsoleInput(io_service, std::move(client), std::move(dispatcher)));
}

void ConsoleInput::Start(void) {
    async_read_until(input_,
                     message_,
                     '\n',
                     [self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                         self->HandleRead(error, bytes_transferred);
                     });
}

ConsoleInput::ConsoleInput(boost::asio::io_service &io_service,
                           TcpClient client,
                           CommandDispatcher<TcpClient> dispatcher) :
    dispatcher_(std::move(dispatcher)),
    client_(std::move(client)),
    input_(io_service, ::dup(STDIN_FILENO)) {
}

void ConsoleInput::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
    const size_t args_num = 2;
    std::string messageP;
    std::stringstream ss;
    ss << &message_;
    ss.flush();
    messageP = ss.str();

    eStatus_t status = dispatcher_.ParseRawString(messageP, client_);
    switch(status) {
        case ErrorCodes::eStatus_Ok:
            cout << "Comand has been executed successfully" << endl;
            break;
        case ErrorCodes::eStatus_WrongArgsNum:
            cout << "Wrong number of command parametrs" << endl;
            break;
        case ErrorCodes::eStatus_GeneralError:
            cout << "Unknown error" << endl;
            break;
        case ErrorCodes::eStatus_LostConnection:
            cout << "Connection has been lost" << endl;
            break;
        default:
            break;
    }

    Start();
}