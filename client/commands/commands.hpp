#ifndef _COMMANDS_H__
#define _COMMANDS_H__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "tcp_client.hpp"
#include "utils.hpp"
#include "parser.hpp"

using std::cout;
using std::endl;

namespace Commands {

    template<class T> class Connect : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &client, const std::vector<std::string> &args) {
            const std::string NAME = "CONNECT";
            const size_t args_num = 2;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                uint16_t port = 0;
                if(Utils::GetPortFromStr(args[0], port)) {
                    cout << "port = " << port << endl;
                    if(eStatus_Ok == client.Connect(port, args[1])) {
                        std::string tmp = NAME + " " + args[1] + "\n";
                        if(client.Write(tmp) == tmp.size()) {
                            ret = eStatus_Ok;
                        }
                    }
                }
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Disconnect : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &client, const std::vector<std::string> &args) {
            const std::string NAME = "DISCONNECT";
            const size_t args_num = 0;
            cout << "size = " << args.size() << endl;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                client.Disconnect();
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Publish : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &client, const std::vector<std::string> &args) {
            const std::string NAME = "PUBLISH";
            const size_t args_num = 2;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() >= args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                std::string tmp = NAME;
                for(auto n : args) {
                    tmp = tmp + " " + n;
                }
                tmp = tmp + "\n";

                if(client.Write(tmp) == tmp.size()) {
                    ret = eStatus_Ok;
                }
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Subscribe : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &client, const std::vector<std::string> &args) {
            const std::string NAME = "SUBSCRIBE";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                std::string tmp = NAME + " " + args[0] + "\n";
                if(client.Write(tmp) == tmp.size()) {
                    ret = eStatus_Ok;
                }
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Unsubscribe : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &client, const std::vector<std::string> &args) {
            const std::string NAME = "UNSUBSCRIBE";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                std::string tmp = NAME + " " + args[0] + "\n";
                if(client.Write(tmp) == tmp.size()) {
                    ret = eStatus_Ok;
                }
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };

} // namespace Commands

#endif /* _COMMANDS_H__*/