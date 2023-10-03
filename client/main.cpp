
#include <_types/_uint16_t.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/termios.h>
#include <optional>
#include "parser.hpp"
#include "commands.hpp"

using boost::bad_lexical_cast;
using boost::lexical_cast;
using std::cout;
using std::endl;

void send_something (std::string host, int port, std::string client_id) {
    boost::asio::io_service ios;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    boost::asio::ip::tcp::socket socket(ios);
    int counter = 0;

    socket.connect(endpoint);

    while(true) {
        std::string msg = "[Client " + client_id + "]:" + std::to_string(counter) + "\n";
        boost::array<char, 512> buf;
        std::copy(msg.begin(), msg.end(), buf.begin());
        boost::system::error_code error;
        socket.write_some(boost::asio::buffer(buf, msg.size()), error);
        cout << msg;
        ++counter;
        sleep(1);
    }

    socket.close();
}

bool CheckPortArgument (std::vector<std::string> args, uint16_t &port) {
    bool ret = false;
    if(args.size() == 1) {
        try {
            int32_t port_32 = lexical_cast<int32_t>(args[0]);
            if(port_32 < 0 || port_32 > UINT16_MAX) {
                throw bad_lexical_cast();
            }
            port = static_cast<uint16_t>(port_32);
            cout << "port: " << port << endl;
            ret = true;

        } catch(bad_lexical_cast &) {
            cout << "Port value is not correct!" << endl;
        }
    } else {
        cout << "Wrong number of arguments!" << endl;
    }
    return ret;
}

void Loop (void) {
    Commands::Connect connect;
    Commands::Disconnect disconnect;
    Commands::Publish publish;
    Commands::Subscribe subscribe;
    Commands::Unsubscribe unsubscribe;
    CommandDispatcher cmd_dispatcher(cout);

    cmd_dispatcher.AddCommand("CONNECT", &connect);
    cmd_dispatcher.AddCommand("DISCONNECT", &disconnect);
    cmd_dispatcher.AddCommand("PUBLISH", &publish);
    cmd_dispatcher.AddCommand("SUBSCRIBE", &subscribe);
    cmd_dispatcher.AddCommand("UNSUBSCRIBE", &unsubscribe);

    while(true) {
        std::string in_line = "";
        std::getline(std::cin, in_line);
        if(in_line != "") {
            bool ret = cmd_dispatcher.ParseRawString(in_line);
            if(true == ret) {
                cout << "Success!" << endl;
            } else {
                cout << "Error!" << endl;
            }
            in_line = "";
        }
    }
}

int main (int argc, char *argv[]) {
    uint16_t port = 0;
    int ret_code = EXIT_FAILURE;
    std::vector<std::string> arguments(argv + 1, argv + argc);
    if(CheckPortArgument(arguments, port)) {
        Loop();
        ret_code = EXIT_SUCCESS;
    }
    return ret_code;
}