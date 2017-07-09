#ifndef client_h__
#define client_h__

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

class client_test
{
public:
    client_test();
    ~client_test();
    
public:
    void configure(const std::string& host, const std::string & port);
    void configure_adress ( const std::string& host, const std::string & port);
    void connect();
    void start();
    void initialize_connection();
    void read_async();
    void write_async(std::string const & data);
    void add_query_message(std::string);
    void write_query_on_server();    
    
    void handle_connect ( const boost::system::error_code & error );
    void handle_write ( const boost::system::error_code & error );
    void handle_read ( const boost::system::error_code & error, size_t read_bytes );

private:
    std::string m_host;
    std::string  m_port;
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_endpoint;
    boost::asio::streambuf m_read_buf;
    boost::asio::streambuf m_write_buf;
    boost::thread m_io_service_thread;
    boost::asio::io_service::work m_worker;
    
    std::string m_current_input_string;
    std::deque<std::string> m_output_messages;
    //bool flag_read = false;
    boost::mutex m_mutex;
};


#endif // client_h__
