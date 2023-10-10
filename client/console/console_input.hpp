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

namespace ConsoleIO {

    class ConsoleInput : public boost::enable_shared_from_this<ConsoleInput>
    {
        struct Tag {
        };

       public:
        using pointer = boost::shared_ptr<ConsoleInput>;

        static ConsoleInput::pointer Create (boost::asio::io_service &io_service,
                                             Network::TcpClient client,
                                             Parser::CommandDispatcher<Network::TcpClient> dispatcher);
        ConsoleInput(Tag,
                     boost::asio::io_service &io_service,
                     Network::TcpClient client,
                     Parser::CommandDispatcher<Network::TcpClient> dispatcher);
        void Start (void);

       private:
        void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);
        /* Do not copy! */
        ConsoleInput(const ConsoleInput &) = delete;
        void operator=(const ConsoleInput &) = delete;

        boost::asio::streambuf message_;
        boost::asio::posix::stream_descriptor input_;
        Parser::CommandDispatcher<Network::TcpClient> dispatcher_;
        Network::TcpClient client_;
    };
} // namespace ConsoleIO
#endif /* _KEYBOARD_H__ */