#ifndef _PARSER_H__
#define _PARSER_H__
#include <boost/lexical_cast.hpp>
#include <cstddef>
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
    virtual eStatus_t Execute (T &context, const std::vector<std::string> &args) = 0;
};

template<class T> class CommandDispatcher
{
   public:
    using StorageType = std::map<std::string, ICommand<T> *>;

    CommandDispatcher(){

    };

    eStatus_t AddCommand (const std::string &name, ICommand<T> &command) {
        eStatus_t ret = eStatus_GeneralError;
        typename StorageType::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair == map_.end()) {
            map_[name] = &command;
            ret = eStatus_Ok;
        } else {
            std::cout << "Command already exists!" << std::endl;
        }
        return ret;
    }

    eStatus_t ParseRawString (const std::string &raw_str, T &context) {
        eStatus_t ret = eStatus_GeneralError;
        const std::string del = " ";
        std::string found = "";
        std::string last(raw_str);
        std::vector<std::string> args;
        while(last != "") {
            if(found == "") {
                found = FindString(last, del);
            } else {
                args.push_back(FindString(last, del));
            }
        }
        if(found != "") {
            ret = Dispatch(found, args, context);
        }
        return ret;
    }

    std::string FindString (std::string &s, const std::string &del) {
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

    CommandDispatcher(CommandDispatcher &&moved) noexcept {
        map_ = std::move(moved.map_);
    }

   private:
    eStatus_t Dispatch (const std::string &name, const std::vector<std::string> &args, T &context) {
        eStatus_t ret = eStatus_GeneralError;
        typename StorageType::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair != map_.end()) {
            ret = map_[name]->Execute(context, args);

        } else {
            std::cout << "This command is not present in the system!" << std::endl;
        }
        return ret;
    }

    /* Do not copy! */
    CommandDispatcher(const CommandDispatcher &) = delete;
    void operator=(const CommandDispatcher &) = delete;

    StorageType map_;
};

#endif /*_PARSER_H__*/