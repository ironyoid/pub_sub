
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

void send_something (std::string host, int port, std::string client_id) {
    boost::asio::io_service ios;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    boost::asio::ip::tcp::socket socket(ios);
    int counter = 0;

    socket.connect(endpoint);

    while(true) {
        std::string msg = "[Client " + client_id + "]:" + std::to_string(counter) + "\n";
        boost::array<char, 512> buf;
        std::copy(msg.begin(), msg.end(), buf.begin());
        boost::system::error_code error;
        socket.write_some(boost::asio::buffer(buf, msg.size()), error);
        cout << msg;
        ++counter;
        sleep(1);
    }

    socket.close();
}

int main (int argc, char *argv[]) {
    if(argc == 2) {
        send_something("127.0.0.1", 1936, std::string(argv[1]));
    }
    return 0;
}