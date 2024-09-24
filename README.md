# Simple Pub/Sub 
### Overview
* Writen in C++17
* Boost::Asio as a network managment framework
* Built and tested with MacOS but also should be compatible with other POSIX systems, but not with Windows

The project is based on `cmake` and includes two targets:
* `p3_client` - tcp client
* `p3_server` - tcp server

All shared code is present in `common` folder and linked as a static lib.
### How To Build And Run
* Generate Makefiles: `cmake  -S . -B build -G "Unix Makefiles"`
* Run build `cmake --build build --config Debug --target all`

### How to use
#### Server
* Run server `./p3_server` with `port` argument, e.g: `./p3_server 1936`
* You are able to monitor logs over terminal during the session

#### Client
* Run client `./p3_client` with `ip address` argument, e.g: `./p3_client 127.0.0.1`
* Use commands from the list below to manage your session

#### Client Commands
* `help` - print help

* `CONNECT [port] [name]` - Connect to a server with the target port, and client name
    * port - server port (1-65535)
    * name - client name (string witout spaces)

* `SUBSCRIBE [topic]` - Subscribe to the topic (if there is no topic with the target name it will be created)
    * topic - topic name (string witout spaces)

* `UNSUBSCRIBE [topic]` - Unsubscribe from the topic
    * topic - topic name (string witout spaces)

* `PUBLISH [topic] [data]` - Publish data into the topic
    * topic - topic name (string witout spaces)
    * data - data to be published (spaces are allowed)

* `DISCONNECT` - Disconnect from the server




