

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/termios.h>
#include <optional>
#include "parser.hpp"
#include "commands.hpp"
#include "tcp_client.hpp"

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

void Loop (std::string &addr) {
    Commands::Connect connect;
    Commands::Disconnect disconnect;
    Commands::Publish publish;
    Commands::Subscribe subscribe;
    Commands::Unsubscribe unsubscribe;
    TcpClient client(addr);
    CommandDispatcher cmd_dispatcher(client);

    cmd_dispatcher.AddCommand("CONNECT", &connect);
    cmd_dispatcher.AddCommand("DISCONNECT", &disconnect);
    cmd_dispatcher.AddCommand("PUBLISH", &publish);
    cmd_dispatcher.AddCommand("SUBSCRIBE", &subscribe);
    cmd_dispatcher.AddCommand("UNSUBSCRIBE", &unsubscribe);

    while(true) {
        std::string in_line = "";
        std::getline(std::cin, in_line);
        if(in_line != "") {
            eStatus_t status = cmd_dispatcher.ParseRawString(in_line);
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
                default:
                    break;
            }

            in_line = "";
        }
    }
}

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;
    std::vector<std::string> arguments(argv + 1, argv + argc);
    if(CheckAddrArgument(arguments)) {
        Loop(arguments[0]);
        ret_code = EXIT_SUCCESS;
    }
    return ret_code;
}