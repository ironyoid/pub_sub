#ifndef _COMMANDS_H__
#define _COMMANDS_H__
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "tcp_client.hpp"
#include "utils.hpp"
#include "parser.hpp"

namespace Commands {

    template<class T> class Connect : public Parser::ICommand<T>
    {
       public:
        ErrorCodes::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "CONNECT";
            const size_t args_num = 2;
            ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
            if(args.size() == args_num) {
                auto port = Utils::GetPortFromStr(args[0]);
                if(port) {
                    ret = context.Connect(port.value(), args[1]);
                } else {
                    ret = ErrorCodes::eStatus_WrongPort;
                }
            } else {
                ret = ErrorCodes::eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Disconnect : public Parser::ICommand<T>
    {
       public:
        ErrorCodes::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "DISCONNECT";
            const size_t args_num = 0;
            ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
            if(args.size() == args_num) {
                ret = context.Disconnect();
            } else {
                ret = ErrorCodes::eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Publish : public Parser::ICommand<T>
    {
       public:
        ErrorCodes::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "PUBLISH";
            const size_t args_num = 2;
            ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
            if(args.size() >= args_num) {
                std::string tmp = NAME;
                for(auto n : args) {
                    tmp = tmp + " " + n;
                }
                tmp = tmp + "\n";
                if(false == context.weak_connection.expired()) {
                    auto shr_tmp = context.weak_connection.lock();
                    if(shr_tmp->Write(tmp) == tmp.size()) {
                        ret = ErrorCodes::eStatus_Ok;
                    }
                } else {
                    ret = ErrorCodes::eStatus_LostConnection;
                }
            } else {
                ret = ErrorCodes::eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Subscribe : public Parser::ICommand<T>
    {
       public:
        ErrorCodes::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "SUBSCRIBE";
            const size_t args_num = 1;
            ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
            if(args.size() == args_num) {
                std::string tmp = NAME + " " + args[0] + "\n";
                if(false == context.weak_connection.expired()) {
                    auto shr_tmp = context.weak_connection.lock();
                    if(shr_tmp->Write(tmp) == tmp.size()) {
                        ret = ErrorCodes::eStatus_Ok;
                    }
                } else {
                    ret = ErrorCodes::eStatus_LostConnection;
                }
            } else {
                ret = ErrorCodes::eStatus_WrongArgsNum;
            }
            return ret;
        }
    };
    template<class T> class Unsubscribe : public Parser::ICommand<T>
    {
       public:
        ErrorCodes::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "UNSUBSCRIBE";
            const size_t args_num = 1;
            ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
            if(args.size() == args_num) {
                std::string tmp = NAME + " " + args[0] + "\n";
                if(false == context.weak_connection.expired()) {
                    auto shr_tmp = context.weak_connection.lock();
                    if(shr_tmp->Write(tmp) == tmp.size()) {
                        ret = ErrorCodes::eStatus_Ok;
                    }
                } else {
                    ret = ErrorCodes::eStatus_LostConnection;
                }
            } else {
                ret = ErrorCodes::eStatus_WrongArgsNum;
            }
            return ret;
        }
    };

} // namespace Commands

#endif /* _COMMANDS_H__*/