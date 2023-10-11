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
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() == args_num) {
                context.ptr->name = args[1];
                LOG_NO_INPUT("SYS", "[" << args[1] << "] has been connected");
                ret = ErrorStatus::eStatus_t::Ok;
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
        static constexpr size_t args_num = 2;
    };

    template<class T> class Publish : public Parser::ICommand<T>
    {
       public:
        ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            if(args.size() >= args_num) {
                std::string tmp = args[2];
                for(auto i = args.begin() + 3; i != args.end(); ++i) {
                    tmp = tmp + " " + *i;
                }
                context.broker.Notify(args[1], tmp);
                ret = ErrorStatus::eStatus_t::Ok;
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
                ret = context.broker.Subscribe(args[1], context.ptr);
                if(ErrorStatus::eStatus_t::Ok == ret) {
                    context.broker.Print();
                    context.ptr->topics_.insert(args[1]);
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
                ret = context.broker.Unsubscribe(args[1], context.ptr);
                if(ErrorStatus::eStatus_t::Ok == ret) {
                    context.broker.Print();
                    context.ptr->topics_.erase(args[1]);
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

#endif /* _COMMANDS_H__ */