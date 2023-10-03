#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "parser.hpp"
using std::cout;
using std::endl;

namespace Commands {

    typedef enum {
        eCommandsState_Ok = 0,
        eCommandsState_WrongArgsNum,
        eCommandsState_GeneralError,
    } eCommandsState_t;

    class ICommand
    {
       public:
        virtual ~ICommand(){};
        virtual eCommandsState_t Execute (std::ostream &output, std::vector<std::string> &args) = 0;
    };

    class Connect : public ICommand
    {
       public:
        eCommandsState_t Execute (std::ostream &output, std::vector<std::string> &args) {
            const size_t args_num = 2;
            eCommandsState_t ret = eCommandsState_GeneralError;
            if(args.size() == args_num) {
                output << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eCommandsState_Ok;
            } else {
                ret = eCommandsState_WrongArgsNum;
            }

            return ret;
        }
    };
    class Disconnect : public ICommand
    {
       public:
        eCommandsState_t Execute (std::ostream &output, std::vector<std::string> &args) {
            const size_t args_num = 0;
            cout << "size = " << args.size() << endl;
            eCommandsState_t ret = eCommandsState_GeneralError;
            if(args.size() == args_num) {
                output << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eCommandsState_Ok;
            } else {
                ret = eCommandsState_WrongArgsNum;
            }

            return ret;
        }
    };
    class Publish : public ICommand
    {
       public:
        eCommandsState_t Execute (std::ostream &output, std::vector<std::string> &args) {
            const size_t args_num = 2;
            eCommandsState_t ret = eCommandsState_GeneralError;
            if(args.size() == args_num) {
                output << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eCommandsState_Ok;
            } else {
                ret = eCommandsState_WrongArgsNum;
            }

            return ret;
        }
    };
    class Subscribe : public ICommand
    {
       public:
        eCommandsState_t Execute (std::ostream &output, std::vector<std::string> &args) {
            const size_t args_num = 1;
            eCommandsState_t ret = eCommandsState_GeneralError;
            if(args.size() == args_num) {
                output << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eCommandsState_Ok;
            } else {
                ret = eCommandsState_WrongArgsNum;
            }

            return ret;
        }
    };
    class Unsubscribe : public ICommand
    {
       public:
        eCommandsState_t Execute (std::ostream &output, std::vector<std::string> &args) {
            const size_t args_num = 1;
            eCommandsState_t ret = eCommandsState_GeneralError;
            if(args.size() == args_num) {
                output << typeid(this).name() << " with " << args.size() << " agrs" << endl;
                ret = eCommandsState_Ok;
            } else {
                ret = eCommandsState_WrongArgsNum;
            }

            return ret;
        }
    };

} // namespace Commands