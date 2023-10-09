#ifndef _COMMANDS_H__
#define _COMMANDS_H__
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"
#include "parser.hpp"
#include "logs.hpp"

namespace Commands {

    template<class T> class Connect : public Parser::ICommand<T>
    {
       public:
        ErrorCodes::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "CONNECT";
            const size_t args_num = 1;
            ErrorCodes::eStatus_t ret = ErrorCodes::eStatus_GeneralError;
            if(args.size() == args_num) {
                context.ptr->name = args[0];
                LOG_NO_INPUT("SYS", "[" << args[0] << "] has been connected");
                ret = ErrorCodes::eStatus_Ok;
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
                std::string tmp = args[1];
                for(auto i = args.begin() + 2; i != args.end(); ++i) {
                    tmp = tmp + " " + *i;
                }
                context.broker.Notify(args[0], tmp);
                ret = ErrorCodes::eStatus_Ok;
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
                ret = context.broker.Subscribe(args[0], context.ptr);
                if(ErrorCodes::eStatus_Ok == ret) {
                    context.broker.Print();
                    context.ptr->topics_.insert(args[0]);
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
                ret = context.broker.Unsubscribe(args[0], context.ptr);
                if(ErrorCodes::eStatus_Ok == ret) {
                    context.broker.Print();
                    context.ptr->topics_.erase(args[0]);
                }
            } else {
                ret = ErrorCodes::eStatus_WrongArgsNum;
            }

            return ret;
        }
    };

} // namespace Commands

#endif /* _COMMANDS_H__ */