# P3 Mobility Code Assignment
### Overview
Project programming language is C++17. Boost::Asio has been chosen as a framework to manage network.
I build and test on MacOS but the project should be also compatible with any POSIX system but not with Windows.
The project is based on `cmake` and includes two targets:
* `p3_client` - tcp client implements all task requerements
* `p3_server` - tcp client implements all task requerements
All common code is present in `common` folder and linked as a static lib.
### How To Build And Run
* Generate Makefiles: `cmake  -S . -B build -G "Unix Makefiles"`
* Run build `cmake --build build --config Debug --target all`

###How to use
#### Server
* Run server `./p3_server` with `port` argument, e.g: `./p3_server 1936`
* You are able to monitor log information during the session

#### Client
* Run client `./p3_client` with `ip address` argument, e.g: `./p3_client 127.0.0.1`
* Use commands from the list below to manage your session

#### Client Commands
* `CONNECT [port] [name]` - Connect to server with the target port, and client name
    * port - server port (1-65535)
    * name - client name (string witout spaces)

* `SUBSCRIBE [topic]` - Subscribe to the topic (if there is no topic wuth target name it will be created)
    * topic - topic name (string witout spaces)

* `UNSUBSCRIBE [topic]` - Unsubscribe from topic
    * topic - topic name (string witout spaces)

* `DISCONNECT` - Disconnect from server




