#include "server.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

server::server()
  : m_socket ( m_io_service ),
    m_acceptor ( m_io_service ) 
{
    
}
server::~server()
{
    m_io_service.stop();
}

void server::configure(int port)
{
    boost::asio::ip::tcp::endpoint epo ( boost::asio::ip::tcp::v4(), port );
    m_acceptor.open ( epo.protocol() );
    m_acceptor.set_option ( boost::asio::socket_base::reuse_address ( true ) );
    m_acceptor.bind ( epo );
    m_acceptor.listen();
}

void server::start()
{
    start_accept();
    m_io_service_thread = boost::thread ( boost::bind ( &boost::asio::io_service::run, &m_io_service ) );
}

void server::start_accept()
{
    m_acceptor.async_accept ( m_socket,
                              boost::bind ( &server::handle_accept, this, boost::asio::placeholders::error ) );
}

void server::read_async()
{
    boost::asio::async_read ( m_socket, m_read_buf, boost::asio::transfer_at_least ( 1 ),
                              boost::bind ( &server::handle_read, this, _1, _2 ) );
}

void server::write_async(const std::string& write_data)
{
    boost::asio::async_write ( m_socket, boost::asio::buffer ( write_data ), boost::bind ( &server::handle_write, this, _1 ) );
}

void server::handle_accept(const boost::system::error_code& error)
{
    std::cout << "Accepted error "<< error.value() <<std::endl;
    if ( error.value() == 0 ) {
        read_async();
    }
}

void server::handle_read(const boost::system::error_code& error, size_t read_bytes)
{
    std::cout << " Read error = " << error.value() << std::endl;
    if (!error.value())
    {   
	std::istream stream_reader ( &m_read_buf );
	std::string local_data ( read_bytes, '\0' );
	stream_reader.read ( &local_data[0], read_bytes );
	m_current_input_string = local_data;	
	std::cout << "Read OK size = " << read_bytes <<std::endl;  
	read_async();
	std::cout << m_current_input_string << std::endl;
        std::cout << "Bytes: ";
	for (int i = 0; i < m_current_input_string.size(); i++){
	    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)(m_current_input_string[i]&0xff) << " " ;
	}
	std::cout << std::endl;
	m_current_input_string = "";
    }
}

void server::handle_write(const boost::system::error_code& error)
{
    std::cout << "Write result. error " << error.value() << std::endl;
    read_async();
}

