#include "udp_server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

udp_server::udp_server(short port,
        int nthreads,
        int blocksz)
        : socket_(io_service_, udp::endpoint(udp::v4(), port)),
          thread_pool_size_(nthreads),
          signals_(io_service_),
          blocksize(blocksz) {
    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&udp_server::handle_stop, this));
    data_ = new char[blocksz];
    std::cout << "Started UDP server "<<std::endl;
}

void udp_server::handle_receive_from(const boost::system::error_code &error,
                                     size_t bytes_recvd) {
    if (!error && bytes_recvd > 0) {
        //std::cout << "Recieved request sz = "<< bytes_recvd << std::endl;
        socket_.async_send_to(
                boost::asio::buffer(data_, bytes_recvd), sender_endpoint_,
                boost::bind(&udp_server::handle_send_to, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }
    else {
        /// Handle error on the receive by closing the socket
        socket_.close();
        std::cout << "Closing connection"<< std::endl;
    }
}

void udp_server::handle_send_to(const boost::system::error_code &error,
                                size_t bytes_sent) {
    if (!error) {
        //std::cout << "Sending response sz = "<< bytes_sent << std::endl;
        socket_.async_receive_from(
                boost::asio::buffer(data_, blocksize), sender_endpoint_,
                boost::bind(&udp_server::handle_receive_from, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } else {
        /// send failed better close the socket?
        socket_.close();
        std::cout << "Closing connection"<< std::endl;
    }
}

void udp_server::run() {
    // Create a pool of threads to run all of the io_services.
    std::vector <boost::shared_ptr<boost::thread>> threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i) {
        boost::shared_ptr <boost::thread> thread(new boost::thread(
                boost::bind(&boost::asio::io_service::run, &io_service_)));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();

    /// Do this after the threads have started
    socket_.async_receive_from(
            boost::asio::buffer(data_, blocksize), sender_endpoint_,
            boost::bind(&udp_server::handle_receive_from, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    io_service_.run();
}

void udp_server::handle_stop()
{
    io_service_.stop();
    std::cout <<"Stopped the io service "<<std::endl;
}