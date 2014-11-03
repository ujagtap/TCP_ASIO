#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::udp;

/// The top-level class of the UDP server.
class udp_server: private boost::noncopyable
{
public:
    udp_server(short port, int nthreads, int blocksize);

    /// run method for the server
    void run();

private:

    /// The io service used by the tcp server
    boost::asio::io_service io_service_;

    /// Number of threads participating in a thread pool
    int thread_pool_size_;

    /// Handle request to stop the server
    void handle_stop();

    /// The signal_set is used to register for process termination notifications.
    boost::asio::signal_set signals_;

    /// Block Size for io
    int blocksize;

    /// UDP socket
    udp::socket socket_;

    /// UDP remote endpoint
    udp::endpoint sender_endpoint_;

    /// Buffer for IO
    char *data_;

    void handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd);

    void handle_send_to(const boost::system::error_code &error, size_t bytes_sent);
};

#endif //UDP_SERVER_HPP