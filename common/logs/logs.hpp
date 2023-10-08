#ifndef _LOGS_H__
#define _LOGS_H__
#include <iostream>
#include <string>

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

inline void printList (std::ostream &os, None) {
}

template<typename Begin, typename Last> void printList (std::ostream &os, const Pair<Begin, Last> &data) {
    printList(os, data.first);
    os << data.second;
}

template<typename List> void log (const char *file, int line, const LogData<List> &data) {
    std::cout << "[" << file << "]: ";
    printList(std::cout, data.list);
    std::cout << "\n"
              << "[IN]-> ";
    std::cout.flush();
}

template<typename List> void log_no_console (const char *file, int line, const LogData<List> &data) {
    std::cout << "[" << file << "]: ";
    printList(std::cout, data.list);
    std::cout << "\n";
    std::cout.flush();
}

template<typename List> void log_erase (const char *file, int line, const LogData<List> &data) {
    std::cout << "\r";
    std::cout << "[" << file << "]: ";
    printList(std::cout, data.list);
    std::cout << "\n"
              << "[IN]-> ";
    std::cout.flush();
}

#define LOG(module, x)            (log(module, __LINE__, LogData<None>() << x))
#define LOG_ERASE(module, x)      (log_erase(module, __LINE__, LogData<None>() << x))
#define LOG_NO_CONSOLE(module, x) (log_no_console(module, __LINE__, LogData<None>() << x))

#endif /* _LOGS_H__ */