#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <istream>
#include <ostream>
#include <string>
#include <random>
#include <assert.h>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
    using boost::asio::ip::udp;
    try
    {
        boost::asio::io_service io_service;
        udp::socket s(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), "localhost", "11949");
        udp::resolver::iterator iterator = resolver.resolve(query);

        int  niterations = boost::lexical_cast<int>(argv[1]);
        int  blocksize   = boost::lexical_cast<int>(argv[2]);

        char reply[blocksize];
        char request[blocksize];

        auto t1 = std::chrono::high_resolution_clock::now();

        for (int k = 0; k < niterations; k++) {
            s.send_to(boost::asio::buffer(request, blocksize), *iterator);
            //std::cout<<"Sent data sz = "<<blocksize<<std::endl;
            udp::endpoint sender_endpoint;
            s.receive_from(boost::asio::buffer(reply, blocksize), sender_endpoint);
            //std::cout<<"Received data sz = "<<blocksize<<std::endl;
        }

        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout <<std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count()<< " seconds.\n" ;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}