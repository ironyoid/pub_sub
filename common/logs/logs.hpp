#ifndef _LOGS_H__
#define _LOGS_H__
#include <iostream>
#include <string>

namespace {

    struct None {
    };

    template<typename First, typename Second> struct Pair {
        First first;
        Second second;
    };

    template<typename List> struct LogData {
        List list;
    };

    template<typename Begin, typename Value>
    LogData<Pair<Begin, const Value &>> operator<<(LogData<Begin> begin, const Value &value) {
        return { { begin.list, value } };
    }

    template<typename Begin, size_t n>
    LogData<Pair<Begin, const char *>> operator<<(LogData<Begin> begin, const char (&value)[n]) {
        return { { begin.list, value } };
    }

    inline void PrintList (std::ostream &os, None) {
    }

    template<typename Begin, typename Last> void PrintList (std::ostream &os, const Pair<Begin, Last> &data) {
        PrintList(os, data.first);
        os << data.second;
    }

    template<typename List> void Log (const char *file, const LogData<List> &data) {
        std::cout << "[" << file << "]: ";
        PrintList(std::cout, data.list);
        std::cout << "\n"
                  << "[IN]-> ";
        std::cout.flush();
    }

    template<typename List> void LogNoInput (const char *file, const LogData<List> &data) {
        std::cout << "[" << file << "]: ";
        PrintList(std::cout, data.list);
        std::cout << "\n";
        std::cout.flush();
    }

    template<typename List> void LogErase (const char *file, const LogData<List> &data) {
        std::cout << "\r";
        std::cout << "[" << file << "]: ";
        PrintList(std::cout, data.list);
        std::cout << "\n"
                  << "[IN]-> ";
        std::cout.flush();
    }

} // namespace
#define LOG(module, x)          (Log(module, LogData<None>() << x))
#define LOG_ERASE(module, x)    (LogErase(module, LogData<None>() << x))
#define LOG_NO_INPUT(module, x) (LogNoInput(module, LogData<None>() << x))
#endif /* _LOGS_H__ */