#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "server.hpp"
#include "udp_server.hpp"

int main(int argc, char *argv[]) {
    try {
        int nthreads  = boost::lexical_cast<int>(argv[2]);
        int blocksize = boost::lexical_cast<int>(argv[3]);
        std::string server_type = argv[1];

        if (server_type == "udp") {
            udp_server udps(11949, 0, blocksize);
            udps.run();
        }
        else {
            http::server3::server tcp_server("localhost", "11849", nthreads, blocksize);
            tcp_server.run();
        }
    } catch (std::exception e) {
        std::cerr<<"exception: "<< e.what() << "\n";
    }
}