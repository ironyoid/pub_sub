

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/termios.h>
#include <optional>
#include "parser.hpp"
#include "commands.hpp"
#include "tcp_client.hpp"
#include "keyboard.hpp"

using std::cout;
using std::endl;

bool CheckAddrArgument (std::vector<std::string> args) {
    bool ret = false;
    if(args.size() == 1) {
        try {
            boost::asio::ip::address addr = boost::asio::ip::address::from_string(args[0]);
            cout << args[0] << " Has been assigned" << endl;
            ret = true;
        } catch(std::exception &e) {
            cout << "Wrong IP address format!" << endl;
        }
    }
    return ret;
}
// void Test (boost::asio::io_service &io_service, std::string &arg, CommandDispatcher<TcpClient> &cmd_dispatcher) {
//     TcpClient::pointer client = TcpClient::Create(io_service, arg);
//     KeyBoardRoutine::pointer keyboard = KeyBoardRoutine::Create(io_service, client, cmd_dispatcher, 10);
//     client->Start();
//     keyboard->Start();
// }

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;
    Commands::Connect<TcpConnection> connect;
    Commands::Disconnect<TcpConnection> disconnect;
    Commands::Publish<TcpConnection> publish;
    Commands::Subscribe<TcpConnection> subscribe;
    Commands::Unsubscribe<TcpConnection> unsubscribe;
    CommandDispatcher<TcpConnection> cmd_dispatcher{};

    cmd_dispatcher.AddCommand("CONNECT", connect);
    cmd_dispatcher.AddCommand("DISCONNECT", disconnect);
    cmd_dispatcher.AddCommand("PUBLISH", publish);
    cmd_dispatcher.AddCommand("SUBSCRIBE", subscribe);
    cmd_dispatcher.AddCommand("UNSUBSCRIBE", unsubscribe);

    std::vector<std::string> arguments(argv + 1, argv + argc);
    if(CheckAddrArgument(arguments)) {
        boost::asio::io_service io_service;
        TcpClient client(io_service, arguments[0]);
        KeyBoardRoutine::pointer keyboard = KeyBoardRoutine::Create(io_service, client, cmd_dispatcher);
        keyboard->Start();
        io_service.run();
        ret_code = EXIT_SUCCESS;
    }
    return ret_code;
}