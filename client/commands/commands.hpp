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
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() == args_num) {
                auto port = Utils::GetPortFromStr(args[1]);
                if(port) {
                    ret = context.Connect(port.value(), args[2]);
                } else {
                    ret = ErrorStatus::eStatus_t::WrongPort;
                }
            } else {
                ret = ErrorStatus::eStatus_t::WrongArgsNum;
            }

            return ret;
        }
        const std::string GetName (void) {
            return name;
        }

       private:
        static const inline std::string name = "CONNECT";
        static constexpr size_t args_num = 3;
    };
    template<class T> class Disconnect : public Parser::ICommand<T>
    {
       public:
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() == args_num) {
                ret = context.Disconnect();
            } else {
                ret = ErrorStatus::eStatus_t::WrongArgsNum;
            }

            return ret;
        }
        const std::string GetName (void) {
            return name;
        }

       private:
        static const inline std::string name = "DISCONNECT";
        static constexpr size_t args_num = 1;
    };
    template<class T> class Publish : public Parser::ICommand<T>
    {
       public:
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() >= args_num) {
                std::string tmp = name;
                for(auto i = args.begin() + 1; i != args.end(); ++i) {
                    tmp = tmp + " " + *i;
                }
                tmp = tmp + "\n";
                if(context.tcp_connection->Write(tmp) == tmp.size()) {
                    ret = ErrorStatus::eStatus_t::Ok;
                }
            } else {
                ret = ErrorStatus::eStatus_t::WrongArgsNum;
            }
            return ret;
        }
        const std::string GetName (void) {
            return name;
        }

       private:
        static const inline std::string name = "PUBLISH";
        static constexpr size_t args_num = 3;
    };

    template<class T> class Subscribe : public Parser::ICommand<T>
    {
       public:
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() == args_num) {
                std::string tmp = name + " " + args[1] + "\n";

                if(context.tcp_connection->Write(tmp) == tmp.size()) {
                    ret = ErrorStatus::eStatus_t::Ok;
                }

            } else {
                ret = ErrorStatus::eStatus_t::WrongArgsNum;
            }
            return ret;
        }
        const std::string GetName (void) {
            return name;
        }

       private:
        static const inline std::string name = "SUBSCRIBE";
        static constexpr size_t args_num = 2;
    };
    template<class T> class Unsubscribe : public Parser::ICommand<T>
    {
       public:
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() == args_num) {
                std::string tmp = name + " " + args[1] + "\n";
                if(context.tcp_connection->Write(tmp) == tmp.size()) {
                    ret = ErrorStatus::eStatus_t::Ok;
                }
            } else {
                ret = ErrorStatus::eStatus_t::WrongArgsNum;
            }
            return ret;
        }
        const std::string GetName (void) {
            return name;
        }

       private:
        static const inline std::string name = "UNSUBSCRIBE";
        static constexpr size_t args_num = 2;
    };

} // namespace Commands

#endif /* _COMMANDS_H__*/