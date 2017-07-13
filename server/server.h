#ifndef server_h__
#define server_h__

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

class server
{
public:
    server ();
    ~server();

public:
    void configure (int port = 8084);
    void start();
    void start_accept();
    void read_async();
    void write_async(std::string const & write_data);

    
public:
    void handle_write ( const boost::system::error_code & err );
    void handle_read ( const boost::system::error_code & err, size_t read_bytes );
    void handle_accept ( const boost::system::error_code & error );
    
private:
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_read_buf;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::thread m_io_service_thread;
    
    std::string m_current_input_string;
    std::ofstream outfile;
    int length_file;
    size_t file_size;
};


#endif // server_h__
