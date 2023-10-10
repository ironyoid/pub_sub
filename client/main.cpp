#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/termios.h>
#include <optional>
#include "parser.hpp"
#include "commands.hpp"
#include "tcp_client.hpp"
#include "console_input.hpp"
#include "utils.hpp"
#include "logs.hpp"
#include <memory>

using ConsoleIO::ConsoleInput;
using Network::TcpClient;
using Parser::CommandDispatcher;
using std::unique_ptr;
using namespace Commands;

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;

    unique_ptr<Connect<TcpClient>> connect = unique_ptr<Connect<TcpClient>>(new Connect<TcpClient>());
    unique_ptr<Disconnect<TcpClient>> disconnect = unique_ptr<Disconnect<TcpClient>>(new Disconnect<TcpClient>());
    unique_ptr<Publish<TcpClient>> publish = unique_ptr<Publish<TcpClient>>(new Publish<TcpClient>());
    unique_ptr<Subscribe<TcpClient>> subscribe = unique_ptr<Subscribe<TcpClient>>(new Subscribe<TcpClient>());
    unique_ptr<Unsubscribe<TcpClient>> unsubscribe = unique_ptr<Unsubscribe<TcpClient>>(new Unsubscribe<TcpClient>());
    CommandDispatcher<TcpClient> cmd_dispatcher{};

    cmd_dispatcher.AddCommand(std::move(connect));
    cmd_dispatcher.AddCommand(std::move(disconnect));
    cmd_dispatcher.AddCommand(std::move(publish));
    cmd_dispatcher.AddCommand(std::move(subscribe));
    cmd_dispatcher.AddCommand(std::move(unsubscribe));

    std::vector<std::string> arguments(argv + 1, argv + argc);
    if(Utils::CheckAddrArgument(arguments)) {
        LOG("SYS", "Server IP address: " << arguments[0]);
        boost::asio::io_service io_service;
        TcpClient client(io_service, arguments[0]);
        ConsoleInput::pointer console_routine
            = ConsoleInput::Create(io_service, std::move(client), std::move(cmd_dispatcher));
        console_routine->Start();
        io_service.run();
        ret_code = EXIT_SUCCESS;
    } else {
        LOG("SYS", "Wrong IP address format!");
    }
    return ret_code;
}