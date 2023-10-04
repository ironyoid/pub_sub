#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "parser.hpp"
#include "tcp_client.hpp"
#include "utils.hpp"

using std::cout;
using std::endl;

namespace Commands {

    class ICommand
    {
       public:
        virtual ~ICommand(){};
        virtual eStatus_t Execute (TcpClient &client, std::vector<std::string> &args) = 0;
    };

    class Connect : public ICommand
    {
       public:
        eStatus_t Execute (TcpClient &client, std::vector<std::string> &args) {
            const size_t args_num = 2;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << "Connect command"
                     << " with " << args.size() << " agrs" << endl;
                uint16_t port = 0;
                if(Utils::GetPortFromStr(args[0], port)) {
                    cout << "port = " << port << endl;
                    ret = client.Connect(port, args[1]);
                }
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    class Disconnect : public ICommand
    {
       public:
        eStatus_t Execute (TcpClient &client, std::vector<std::string> &args) {
            const size_t args_num = 0;
            cout << "size = " << args.size() << endl;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << "Disonnect command"
                     << " with " << args.size() << " agrs" << endl;
                client.Disconnect();
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    class Publish : public ICommand
    {
       public:
        eStatus_t Execute (TcpClient &client, std::vector<std::string> &args) {
            const size_t args_num = 2;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    class Subscribe : public ICommand
    {
       public:
        eStatus_t Execute (TcpClient &client, std::vector<std::string> &args) {
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    class Unsubscribe : public ICommand
    {
       public:
        eStatus_t Execute (TcpClient &client, std::vector<std::string> &args) {
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };

} // namespace Commands