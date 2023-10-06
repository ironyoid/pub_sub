

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

class KeyBoardRoutine
{
   public:
    KeyBoardRoutine(TcpClient &client, CommandDispatcher<TcpClient> &dispatcher, uint32_t period) :
        dispatcher_(dispatcher),
        client_(client),
        timer_(client_.io_context, boost::posix_time::milliseconds(period)),
        period_(period) {
    }

    void Start (void) {
        timer_.cancel();
        timer_.expires_from_now(boost::posix_time::milliseconds(period_));
        timer_.async_wait([this] (const boost::system::error_code &error) {
            if(error != boost::asio::error::operation_aborted) {
                this->Handler();
            }
        });
    }

   private:
    void Handler (void) {
        ProcessKeyboard();
        Start();
    }

    void ProcessKeyboard (void) {
        std::string rx_line(1024, ' ');
        char buf[1024];
        std::cin.readsome(buf, 1024);
        std::string in_line(buf);

        // bool cinready = std::cin.peek() != EOF;
        // if(cinready) {
        //     std::getline(std::cin, in_line);
        // } else {
        //     return;
        // }
        if(in_line != "") {
            eStatus_t status = dispatcher_.ParseRawString(in_line, client_);
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

    CommandDispatcher<TcpClient> &dispatcher_;
    TcpClient &client_;
    uint32_t period_;
    boost::asio::deadline_timer timer_;
};

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

int main (int argc, char *argv[]) {
    int ret_code = EXIT_FAILURE;
    Commands::Connect<TcpClient> connect;
    Commands::Disconnect<TcpClient> disconnect;
    Commands::Publish<TcpClient> publish;
    Commands::Subscribe<TcpClient> subscribe;
    Commands::Unsubscribe<TcpClient> unsubscribe;
    CommandDispatcher<TcpClient> cmd_dispatcher{};

    cmd_dispatcher.AddCommand("CONNECT", &connect);
    cmd_dispatcher.AddCommand("DISCONNECT", &disconnect);
    cmd_dispatcher.AddCommand("PUBLISH", &publish);
    cmd_dispatcher.AddCommand("SUBSCRIBE", &subscribe);
    cmd_dispatcher.AddCommand("UNSUBSCRIBE", &unsubscribe);

    std::vector<std::string> arguments(argv + 1, argv + argc);
    if(CheckAddrArgument(arguments)) {
        TcpClient client(arguments[0]);
        KeyBoardRoutine keyboard(client, cmd_dispatcher, 10);
        keyboard.Start();
        client.StartRead();
        client.io_context.run();
        //std::thread t([&] { client.io_context.run(); });
        //client.io_context.run();
        //t.join();
        ret_code = EXIT_SUCCESS;
    }
    return ret_code;
}