

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/termios.h>
#include <optional>
#include "parser.hpp"
#include "commands.hpp"
#include "tcp_client.hpp"
#include <boost/asio/posix/stream_descriptor.hpp>

using std::cout;
using std::endl;

class KeyBoardRoutine : public boost::enable_shared_from_this<KeyBoardRoutine>
{
   public:
    using pointer = boost::shared_ptr<KeyBoardRoutine>;

    static KeyBoardRoutine::pointer Create (TcpClient &client,
                                            CommandDispatcher<TcpClient> &dispatcher,
                                            uint32_t period) {
        return pointer(new KeyBoardRoutine(client, dispatcher, period));
    }

    void Start (void) {
        async_read_until(
            input_,
            message_,
            '\n',
            [self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                self->HandleRead(error, bytes_transferred);
            });
    }

   private:
    KeyBoardRoutine(TcpClient &client, CommandDispatcher<TcpClient> &dispatcher, uint32_t period) :
        dispatcher_(dispatcher),
        client_(client),
        period_(period),
        input_(client.io_context, ::dup(STDIN_FILENO)) {
    }

    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred) {
        std::string messageP;
        {
            std::stringstream ss;
            ss << &message_;
            ss.flush();
            messageP = ss.str();
        }
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
            default:
                break;
        }
        Start();
    }

    boost::asio::posix::stream_descriptor input_;
    CommandDispatcher<TcpClient> &dispatcher_;
    TcpClient &client_;
    uint32_t period_;
    boost::asio::streambuf message_;
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
        TcpClient::pointer client = TcpClient::Create(arguments[0]);
        KeyBoardRoutine::pointer keyboard = KeyBoardRoutine::Create(*client, cmd_dispatcher, 10);
        keyboard->Start();
        client->StartRead();
        client->io_context.run();
        ret_code = EXIT_SUCCESS;
    }
    return ret_code;
}