#include "keyboard.hpp"
#include "parser.hpp"
#include "tcp_client.hpp"
#include "utils.hpp"
#include <iostream>
using std::cout;
using std::endl;

KeyBoardRoutine::pointer KeyBoardRoutine::Create(boost::asio::io_service &io_service,
                                                 TcpClient &client,
                                                 CommandDispatcher<TcpConnection> &dispatcher) {
    return pointer(new KeyBoardRoutine(io_service, client, dispatcher));
}

void KeyBoardRoutine::Start(void) {
    async_read_until(input_,
                     message_,
                     '\n',
                     [self = shared_from_this()] (const boost::system::error_code &error, size_t bytes_transferred) {
                         self->HandleRead(error, bytes_transferred);
                     });
}

KeyBoardRoutine::KeyBoardRoutine(boost::asio::io_service &io_service,
                                 TcpClient &client,
                                 CommandDispatcher<TcpConnection> &dispatcher) :
    state(eKeyboardState_Connect),
    dispatcher_(dispatcher),
    client_(client),
    input_(io_service, ::dup(STDIN_FILENO)) {
}

void KeyBoardRoutine::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
    const size_t args_num = 2;
    TcpConnection::pointer tcp_connection;
    std::string messageP;
    std::stringstream ss;
    ss << &message_;
    ss.flush();
    messageP = ss.str();
    if(eKeyboardState_Connect == state) {
        std::vector<std::string> args;
        std::string name = "";
        std::string del = " ";
        while(messageP != "") {
            if(name == "") {
                name = dispatcher_.FindString(messageP, del);
            } else {
                args.push_back(dispatcher_.FindString(messageP, del));
            }
        }

        if(("CONNECT" == name) && (args_num == args.size())) {
            uint16_t port = 0;
            if(true == Utils::GetPortFromStr(args[0], port)) {
                tcp_connection = client_.Start(*this);
                if(eStatus_Ok == tcp_connection->Connect(port, args[1])) {
                    std::string tmp = "CONNECT " + args[1] + "\n";
                    if(tcp_connection->Write(tmp) == tmp.size()) {
                        tcp_connection->Start();
                        client_ptr_ = tcp_connection;
                        state = eKeyboardState_Process;
                    }
                }
            }
        }
    } else {
        if(false == client_ptr_.expired()) {
            eStatus_t status = dispatcher_.ParseRawString(messageP, *client_ptr_.lock());
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
        } else {
            cout << "Connection has been lost" << endl;
            state = eKeyboardState_Connect;
        }
    }

    Start();
}