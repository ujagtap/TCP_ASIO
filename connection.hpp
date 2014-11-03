#ifndef HTTP_SERVER3_CONNECTION_HPP
#define HTTP_SERVER3_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace http {
    namespace server3 {

        /// Represents a single connection from a client.
        class connection : public boost :: enable_shared_from_this<connection>,
                           private boost :: noncopyable
        {
        public:

            explicit connection(boost:: asio:: io_service &io_service, int blocksz);

            /// Get the socket associated with the connection.
            boost::asio::ip::tcp::socket& socket();

            /// Start the first asynchronous operation for the connection.
            void start();

        private:
            /// Handle completion of a read operation.
            void handle_read(const boost::system::error_code& e,
                             std::size_t bytes_transferred);

            /// Handle completion of a write operation.
            void handle_write(const boost::system::error_code& e);

            /// Strand to ensure the connection's handlers are not called concurrently.
            boost::asio::io_service::strand strand_;

            /// Socket for the connection.
            boost::asio::ip::tcp::socket socket_;

            /// Blocksize for IO
            int blocksize;

            /// Buffer to send the data
            char *buffer;
        };

        /// Connection pointer gets passed to the thread executing the dispatcher
        typedef boost::shared_ptr<connection> connection_ptr;
    }
}

#endif // HTTP_SERVER3_CONNECTION_HPP