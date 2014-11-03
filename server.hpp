#ifndef HTTP_SERVER3_SERVER_HPP
#define HTTP_SERVER3_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"

namespace http {
    namespace server3 {

        /// The top-level class of the TCP server.
        class server: private boost::noncopyable
        {
        public:

            explicit server(const std::string & ipaddress,
                            const std::string &port,
                            std::size_t nthreads,
                            int blocksz);

            /// run method for the server
            void run();

        private:

            /// The io service used by the tcp server
            boost::asio::io_service io_service_;

            /// Number of threads participating in a thread pool
            int thread_pool_size_;

            /// Acceptor used to accept the incoming connections
            boost::asio::ip::tcp::acceptor acceptor_;

            /// Pointer to the next incoming connection
            connection_ptr new_connection_;

            /// Start accepting new connections
            void start_accept();

            /// Handler for new connection
            void handle_accept(const boost::system::error_code &error);

            /// Handle request to stop the server
            void handle_stop();

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Block Size for io
            int blocksize;
        };
    } // namespace server3
} // namespace http

#endif // HTTP_SERVER3_SERVER_HPP