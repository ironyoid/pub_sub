#ifndef _COMMANDS_H__
#define _COMMANDS_H__
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"
#include "parser.hpp"

namespace Commands {

    template<class T> class Connect : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "CONNECT";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                context.ptr->name = args[0];
                std::cout << "[" << args[0] << "] has been connected" << std::endl;
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
        eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "PUBLISH";
            const size_t args_num = 2;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() >= args_num) {
                std::string tmp = args[1];
                for(auto i = args.begin() + 2; i != args.end(); ++i) {
                    tmp = tmp + " " + *i;
                }
                tmp = tmp + "\n";
                context.broker.Notify(args[0], tmp);
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Subscribe : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "SUBSCRIBE";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                ret = context.broker.Subscribe(args[0], context.ptr);
                if(eStatus_Ok == ret) {
                    context.broker.Print();
                    context.ptr->topics_.insert(args[0]);
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
        eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "UNSUBSCRIBE";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                ret = context.broker.Unsubscribe(args[0], context.ptr);
                if(eStatus_Ok == ret) {
                    context.broker.Print();
                    context.ptr->topics_.erase(args[0]);
                }
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };

} // namespace Commands

#endif /* _COMMANDS_H__ */