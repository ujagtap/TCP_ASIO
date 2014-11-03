#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace http {
    namespace server3 {

        connection::connection(boost::asio::io_service& io_service, int blocksz)
                : strand_(io_service),
                  socket_(io_service),
                  blocksize(blocksz) {
            buffer = new char[blocksz];
        }

        boost::asio::ip::tcp::socket& connection::socket() {
            return socket_;
        }

        void connection::start() {
            //std::cout<<"Thread "<< boost::this_thread::get_id() <<" accepted connection"<<std::endl;
            boost::asio::async_read(socket_, boost::asio::buffer(buffer, blocksize),
                            boost::bind(&connection::handle_read,
                                    shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
        }

        void connection::handle_read(const boost::system::error_code& e,
                                     std::size_t bytes_transferred) {
            if (!e) {
                //std::cout<<"Thread "<< boost::this_thread::get_id()<<" got request , blksz = "<< blocksize << std::endl;
                boost::asio::async_write(socket_, boost::asio::buffer(buffer, bytes_transferred),
                                         boost::bind(&connection::handle_write,
                                                     shared_from_this(),
                                                     boost::asio::placeholders::error));
            } else {
                // If an error occurs then no new asynchronous operations are started. This
                // means that all shared_ptr references to the connection object will
                // disappear and the object will be destroyed automatically after this
                // handler returns. The connection class's destructor closes the socket.
                //std::cout<<"Remote closed connection "<<std::endl;
                socket_.close();
            }
        }

        void connection::handle_write(const boost::system::error_code& e) {
            if (!e)
            {
                //std::cout<<"Thread "<< boost::this_thread::get_id()<<" wrote response , blksz = "<< blocksize << std::endl;
                /// Start the next read, as the earlier response completed successfully
                boost::asio::async_read(socket_, boost::asio::buffer(buffer, blocksize),
                                        boost::bind(&connection::handle_read,
                                                    shared_from_this(),
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred));
            } else {
                // No new asynchronous operations are started. This means that all shared_ptr
                // references to the connection object will disappear and the object will be
                // destroyed automatically after this handler returns. The connection class's
                // destructor closes the socket.
                //std::cout<<"Remote closed connection "<<std::endl;
                socket_.close();
            }
        }
    } // namespace server3
} // namespace http