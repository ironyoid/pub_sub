#pragma once
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
        eStatus_t Execute (T &context, std::vector<std::string> &args) {
            const std::string NAME = "CONNECT";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
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
        eStatus_t Execute (T &context, std::vector<std::string> &args) {
            const std::string NAME = "PUBLISH";
            const size_t args_num = 2;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() >= args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
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
        eStatus_t Execute (T &context, std::vector<std::string> &args) {
            const std::string NAME = "SUBSCRIBE";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };
    template<class T> class Unsubscribe : public ICommand<T>
    {
       public:
        eStatus_t Execute (T &context, std::vector<std::string> &args) {
            const std::string NAME = "UNSUBSCRIBE";
            const size_t args_num = 1;
            eStatus_t ret = eStatus_GeneralError;
            if(args.size() == args_num) {
                cout << NAME << " command"
                     << " with " << args.size() << " agrs" << endl;
                ret = eStatus_Ok;
            } else {
                ret = eStatus_WrongArgsNum;
            }

            return ret;
        }
    };

} // namespace Commands