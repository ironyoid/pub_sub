#ifndef _COMMANDS_H__
#define _COMMANDS_H__
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "utils.hpp"
#include "parser.hpp"

using std::cout;
using std::endl;

namespace Commands {

    template<class T> class Connect : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &context, const std::vector<std::string> &args) {
            const std::string NAME = "CONNECT";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                context.ptr->name = args[0];
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
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                std::string tmp = "";
                for(auto i = args.begin() + 1; i != args.end(); i++) {
                    tmp = tmp + " " + *i;
                }
                tmp = tmp + "\n";
                std::cout << "Res string: " << tmp << endl;
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
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                ret = context.broker.Subscribe(args[0], context.ptr);
                if(eStatus_Ok == ret) {
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
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                ret = context.broker.Unsubscribe(args[0], context.ptr);
                if(eStatus_Ok == ret) {
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