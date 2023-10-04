#pragma once
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "commands.hpp"
#include "tcp_client.hpp"

using namespace Commands;
using namespace ErrorCodes;
class CommandDispatcher
{
   public:
    using command_map = std::map<std::string, ICommand *>;

    CommandDispatcher(TcpClient &client) :
        client_(client){

        };

    eStatus_t AddCommand (std::string name, ICommand *command) {
        eStatus_t ret = eStatus_GeneralError;
        command_map::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair == map_.end()) {
            map_[name] = command;
            ret = eStatus_Ok;

        } else {
            std::cout << "Command already exists!" << std::endl;
        }
        return ret;
    }

    eStatus_t ParseRawString (std::string &raw_str) {
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
            ret = Dispatch(name, args);
        }
        return ret;
    }

    eStatus_t Dispatch (std::string &name, std::vector<std::string> &args) {
        eStatus_t ret = eStatus_GeneralError;
        command_map::const_iterator cmd_pair = map_.find(name);
        if(cmd_pair != map_.end()) {
            ret = map_[name]->Execute(client_, args);

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
    TcpClient &client_;
};