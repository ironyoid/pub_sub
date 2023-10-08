#ifndef _KEYBOARD_H__
#define _KEYBOARD_H__
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/termios.h>
#include <optional>
#include "parser.hpp"
#include "tcp_client.hpp"
#include <boost/asio/posix/stream_descriptor.hpp>

class ConsoleInput : public boost::enable_shared_from_this<ConsoleInput>
{
   public:
    using pointer = boost::shared_ptr<ConsoleInput>;

    static ConsoleInput::pointer Create (boost::asio::io_service &io_service,
                                         TcpClient client,
                                         CommandDispatcher<TcpClient> dispatcher);
    void Start (void);

   private:
    ConsoleInput(boost::asio::io_service &io_service, TcpClient client, CommandDispatcher<TcpClient> dispatcher);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);
    /* Do not copy! */
    ConsoleInput(const ConsoleInput &) = delete;
    void operator=(const ConsoleInput &) = delete;

    boost::asio::streambuf message_;
    boost::asio::posix::stream_descriptor input_;
    CommandDispatcher<TcpClient> dispatcher_;
    TcpClient client_;
};
#endif /* _KEYBOARD_H__ */