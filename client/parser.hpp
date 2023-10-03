#pragma once
#include <atomic>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "commands.hpp"

using namespace Commands;
class CommandDispatcher
{
   public:
    using command_map = std::map<std::string, ICommand *>;

    CommandDispatcher(std::ostream &output) :
        output_(output){

        };

    bool AddCommand (std::string name, ICommand *command) {
        bool ret = false;
        command_map::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair == map_.end()) {
            map_[name] = command;
            ret = true;

        } else {
            std::cout << "Command already exists!" << std::endl;
        }
        return ret;
    }

    bool ParseRawString (std::string &raw_str) {
        bool ret = false;
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
            ret = Dispatch(name, args);
        }
        return ret;
    }

    bool Dispatch (std::string &name, std::vector<std::string> &args) {
        bool ret = false;
        command_map::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair != map_.end()) {
            auto tmp = map_[name]->Execute(output_, args);
            if(Commands::eCommandsState_Ok == tmp) {
                ret = true;
            }
        } else {
            std::cout << "This command is not present in the system!" << std::endl;
        }
        return ret;
    }

   private:
    std::string FindString (std::string &s, std::string &del) {
        std::string a;
        int end = s.find(del);
        if(std::string::npos != end) {
            a = s.substr(0, end);
            s.erase(s.begin(), s.begin() + end + del.length());
        } else {
            a = s;
            s = "";
        }
        return a;
    }

    command_map map_;
    std::ostream &output_;
};