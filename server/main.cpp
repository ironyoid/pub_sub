#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <array>

using boost::asio::ip::tcp;
using std::cout;
using std::endl;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
   public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create (const boost::asio::any_io_executor &io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket &socket () {
        return socket_;
    }

    void start () {
        async_read_until(socket_,
                         message_,
                         '\n',
                         boost::bind(&tcp_connection::handle_read,
                                     shared_from_this(),
                                     boost::asio::placeholders::error,
                                     boost::asio::placeholders::bytes_transferred));
    }

   private:
    tcp_connection(const boost::asio::any_io_executor &io_context) : socket_(io_context) {
    }

    void handle_write (const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/) {
    }

    void handle_read (const boost::system::error_code &error, size_t bytes_transferred) {
        if(error && error != boost::asio::error::eof) {
            std::cout << "Error: " << error.message() << "\n";
            return;
        }

        std::string messageP;
        {
            std::stringstream ss;
            ss << &message_;
            ss.flush();
            messageP = ss.str();
        }

        std::cout << messageP;
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(messageP),
                                 boost::bind(&tcp_connection::handle_write,
                                             shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
        start();
    }

    tcp::socket socket_;
    boost::asio::streambuf message_;
};

class tcp_server
{
   public:
    tcp_server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1936)) {
        start_accept();
    }

   private:
    void start_accept () {
        tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_executor());

        acceptor_.async_accept(
            new_connection->socket(),
            boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));
    }

    void handle_accept (tcp_connection::pointer new_connection, const boost::system::error_code &error) {
        if(!error) {
            std::cout << "A client connected" << std::endl;
            new_connection->start();
        }

        start_accept();
    }

    tcp::acceptor acceptor_;
};

int main (int argc, char *argv[]) {
    try {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}