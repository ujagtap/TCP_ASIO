#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <random>
#include <assert.h>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try {

        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("localhost", "11849");
        tcp::resolver::iterator iterator = resolver.resolve(query);
        tcp::socket s(io_service);
        s.connect(*iterator);

        int  niterations = boost::lexical_cast<int>(argv[1]);
        int  blocksize   = boost::lexical_cast<int>(argv[2]);

        char reply[blocksize];
        char request[blocksize];
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution (1, blocksize-1);
        int index = distribution(generator);
        int value = index % 256;
        request[index] = value;

        auto t1 = std::chrono::high_resolution_clock::now();

        for (int k = 0; k < niterations; k++) {
            boost::asio::write(s, boost::asio::buffer(request, blocksize));
            size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, blocksize));
            assert(request[index] == reply[index]);
        }

        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout <<std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count()<< " seconds.\n" ;
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}