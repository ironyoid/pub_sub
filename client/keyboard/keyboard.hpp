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

class KeyBoardRoutine : public boost::enable_shared_from_this<KeyBoardRoutine>
{
   public:
    using pointer = boost::shared_ptr<KeyBoardRoutine>;

    static KeyBoardRoutine::pointer Create (boost::asio::io_service &io_service,
                                            TcpClient &client,
                                            CommandDispatcher<TcpClient> &dispatcher,
                                            uint32_t period);
    void Start (void);

   private:
    KeyBoardRoutine(boost::asio::io_service &io_service,
                    TcpClient &client,
                    CommandDispatcher<TcpClient> &dispatcher,
                    uint32_t period);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);

    boost::asio::posix::stream_descriptor input_;
    CommandDispatcher<TcpClient> &dispatcher_;
    TcpClient &client_;
    uint32_t period_;
    boost::asio::streambuf message_;
};
#endif /* _KEYBOARD_H__ */