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
    typedef enum {
        eKeyboardState_Connect = 0,
        eKeyboardState_Process,
    } eKeyboardState_t;
    using pointer = boost::shared_ptr<KeyBoardRoutine>;

    static KeyBoardRoutine::pointer Create (boost::asio::io_service &io_service,
                                            TcpClient &client,
                                            CommandDispatcher<TcpConnection> &dispatcher);
    void Start (void);
    eKeyboardState_t state;

   private:
    KeyBoardRoutine(boost::asio::io_service &io_service,
                    TcpClient &client,
                    CommandDispatcher<TcpConnection> &dispatcher);
    void HandleRead (const boost::system::error_code &error, size_t bytes_transferred);

    boost::asio::posix::stream_descriptor input_;
    CommandDispatcher<TcpConnection> &dispatcher_;
    TcpConnection::weak_pointer client_ptr_;
    TcpClient &client_;
    boost::asio::streambuf message_;

    //friend class TcpConnection;
};
#endif /* _KEYBOARD_H__ */