#ifndef _PARSER_H__
#define _PARSER_H__
#include <boost/lexical_cast.hpp>
#include <cstddef>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utils.hpp>
#include "logs.hpp"
#include <boost/algorithm/string.hpp>
#include <memory>

namespace Parser {

    template<class T> class ICommand
    {
       public:
        virtual ~ICommand(){};
        virtual ErrorStatus::eStatus_t Execute (T &context, const std::vector<std::string> &args) = 0;
        virtual const std::string GetName (void) = 0;
    };

    template<class T> class CommandDispatcher
    {
       public:
        using ElementType = std::unique_ptr<ICommand<T>>;
        using StorageType = std::map<std::string, ElementType>;

        CommandDispatcher() = default;

        ErrorStatus::eStatus_t AddCommand (ElementType command) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            auto map_itr = map_.find(command->GetName());
            if(map_itr == map_.end()) {
                map_[command->GetName()] = std::move(command);
                ret = ErrorStatus::eStatus_t::Ok;
            } else {
                LOG("SYS", "Command already exists!");
            }
            return ret;
        }

        ErrorStatus::eStatus_t ParseRawString (const std::string &raw_str, T &context) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            auto args = FindString(raw_str);
            if(0 != args.size()) {
                ret = Dispatch(args, context);
            }
            return ret;
        }

        std::vector<std::string> FindString (const std::string &s) {
            std::vector<std::string> result;
            boost::split(result, s, boost::is_any_of(" "));
            if(0 != result.size()) {
                /* Remove \n */
                result.back().pop_back();
            }
            return result;
        }

        CommandDispatcher(CommandDispatcher &&moved) noexcept {
            map_ = std::move(moved.map_);
        }

       private:
        ErrorStatus::eStatus_t Dispatch (const std::vector<std::string> &args, T &context) {
            ErrorStatus::eStatus_t ret = ErrorStatus::eStatus_t::GeneralError;
            auto map_itr = map_.find(args[0]);
            if(map_itr != map_.end()) {
                ret = map_[args[0]]->Execute(context, args);
            } else {
                LOG("SYS", "This command is not present in the system!");
            }
            return ret;
        }

        /* Do not copy! */
        CommandDispatcher(const CommandDispatcher &) = delete;
        void operator=(const CommandDispatcher &) = delete;

        StorageType map_;
    };
} // namespace Parser
#endif /*_PARSER_H__*/