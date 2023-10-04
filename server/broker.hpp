#pragma ones
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <boost/intrusive/list.hpp>
#include <map>
#include "utils.hpp"

using namespace boost::intrusive;
using namespace ErrorCodes;

class Element : public list_base_hook<link_mode<auto_unlink>>
{
   public:
    Element(std::string name) : name_(name) {
    }

    std::string GetName (void) {
        return name_;
    }

   private:
    std::string name_;
};

class Broker
{
   public:
    using ListType = list<Element, constant_time_size<false>>;
    using StorageType = std::map<std::string, ListType>;
    Broker() : storage_() {
    }

    size_t GetNumberOfSubscribers (std::string &topic) {
        size_t ret = 0;
        StorageType::const_iterator cmd_pair = storage_.find(topic);
        if(cmd_pair != storage_.end()) {
            ret = storage_[topic].size();
        }
        return ret;
    }

    eStatus_t Subscribe (std::string &topic, Element *client) {
        eStatus_t ret = eStatus_GeneralError;
        storage_[topic].push_back(*client);
        ret = eStatus_Ok;
        return ret;
    }

    eStatus_t Unsubscribe (std::string &topic, Element *client) {
        eStatus_t ret = eStatus_GeneralError;
        StorageType::const_iterator cmd_pair = storage_.find(topic);
        if(cmd_pair != storage_.end()) {
            for(auto n : storage_[topic]) {
                std::cout << "name = " << n.GetName() << std::endl;
                if(n.GetName() == client->GetName()) {
                    auto it = ListType::s_iterator_to(*client);
                    storage_[topic].erase(it);
                    ret = eStatus_Ok;
                    break;
                }
            }
            std::cout << std::endl;
        }

        // StorageType::const_iterator cmd_pair = storage_.find(topic);

        // auto test = ListType::s_iterator_to(*client);
        // if(test != storage_[topic].end()) {
        //     storage_[topic].erase(test);
        //     ret = eStatus_Ok;
        // } else {
        //     std::cout << "There is no such element" << std::endl;
        // }

        return ret;
    }

   private:
    StorageType storage_;
};