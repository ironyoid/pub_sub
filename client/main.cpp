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

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;
    Commands::Connect<TcpClient> connect;
    Commands::Disconnect<TcpClient> disconnect;
    Commands::Publish<TcpClient> publish;
    Commands::Subscribe<TcpClient> subscribe;
    Commands::Unsubscribe<TcpClient> unsubscribe;
    CommandDispatcher<TcpClient> cmd_dispatcher{};

    cmd_dispatcher.AddCommand("CONNECT", connect);
    cmd_dispatcher.AddCommand("DISCONNECT", disconnect);
    cmd_dispatcher.AddCommand("PUBLISH", publish);
    cmd_dispatcher.AddCommand("SUBSCRIBE", subscribe);
    cmd_dispatcher.AddCommand("UNSUBSCRIBE", unsubscribe);

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