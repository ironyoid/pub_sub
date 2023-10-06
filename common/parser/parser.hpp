#ifndef _PARSER_H__
#define _PARSER_H__
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utils.hpp>

using namespace ErrorCodes;

template<class T> class ICommand
{
   public:
    virtual ~ICommand(){};
    virtual eStatus_t Execute (T &context, std::vector<std::string> &args) = 0;
};

template<class T> class CommandDispatcher
{
   public:
    using StorageType = std::map<std::string, ICommand<T> *>;

    CommandDispatcher(){

    };

    eStatus_t AddCommand (std::string name, ICommand<T> *command) {
        eStatus_t ret = eStatus_GeneralError;
        typename StorageType::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair == map_.end()) {
            map_[name] = command;
            ret = eStatus_Ok;

        } else {
            std::cout << "Command already exists!" << std::endl;
        }
        return ret;
    }

    eStatus_t ParseRawString (std::string &raw_str, T &context) {
        eStatus_t ret = eStatus_GeneralError;
        std::vector<std::string> args;
        std::string name = "";
        std::string del = " ";
        while(raw_str != "") {
            if(name == "") {
                name = FindString(raw_str, del);
            } else {
                args.push_back(FindString(raw_str, del));
            }
        }
        if(name != "") {
            ret = Dispatch(name, args, context);
        }
        return ret;
    }

   private:
    eStatus_t Dispatch (std::string &name, std::vector<std::string> &args, T &context) {
        eStatus_t ret = eStatus_GeneralError;
        typename StorageType::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair != map_.end()) {
            ret = map_[name]->Execute(context, args);

        } else {
            std::cout << "This command is not present in the system!" << std::endl;
        }
        return ret;
    }

    std::string FindString (std::string &s, std::string &del) {
        std::string a;
        int end = s.find(del);
        if(std::string::npos != end) {
            a = s.substr(0, end);
            s.erase(s.begin(), s.begin() + end + del.length());
        } else {
            if(s.back() == '\n') {
                s.pop_back();
            }
            a = s;
            s = "";
        }
        return a;
    }

    StorageType map_;
};

#endif /*_PARSER_H__*/