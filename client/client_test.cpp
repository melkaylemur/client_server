#include "client_test.h"

client_test::client_test()
    : m_socket ( m_io_service ),
      m_worker ( m_io_service )
{

}

client_test::~client_test()
{
    m_io_service.stop();
    m_io_service_thread.join();
}

void client_test::configure(const std::string& host, const std::string& port)
{
    configure_adress(host, port);
    connect();
}

void client_test::configure_adress(const std::string& host, const std::string& port)
{
    m_host = host;
    m_port = port;
    /*if(!m_host.compare("localhost")) {
        m_host = "127.0.0.1";
    }
*/
    boost::asio::ip::tcp::resolver resolver(m_io_service);
    boost::asio::ip::tcp::resolver::query query(m_host, m_port);
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::endpoint ep = *iter;
    m_endpoint = ep;
}

void client_test::connect()
{
    start();
    initialize_connection();
}

void client_test::start()
{
    m_io_service_thread = boost::thread ( boost::bind ( &boost::asio::io_service::run, &m_io_service ) );
}

void client_test::initialize_connection()
{
    m_socket.close();
    m_socket.async_connect( m_endpoint, boost::bind( &client_test::handle_connect, this, boost::asio::placeholders::error) );
}

void client_test::handle_connect(const boost::system::error_code& error)
{
    std::cout << " Connect error = " << error.value() << std::endl;
    if ( !error.value() ){
	read_async();
	boost::lock_guard<boost::mutex> lock( m_mutex );
	if (!m_output_messages.empty()){
	    write_query_on_server();
	}
    }
}

void client_test::read_async()
{
    boost::asio::async_read ( m_socket, m_read_buf, boost::asio::transfer_at_least ( 1 ),
                              boost::bind ( &client_test::handle_read, this, _1, _2 ) );
}

void client_test::handle_read(const boost::system::error_code& error, size_t read_bytes)
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
	
	for (int i = 0; i < m_current_input_string.size(); i++){
	    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)(m_current_input_string[i]&0xff) << " " ;
	}
	std::cout << std::endl;
	m_current_input_string = "";
    }
}

void client_test::write_async(const std::string& write_data)
{
    std::ostream stream_writer(&m_write_buf);
    stream_writer << write_data;
    boost::asio::async_write ( m_socket, m_write_buf, boost::bind ( &client_test::handle_write, this, _1 ) );
}

void client_test::handle_write(const boost::system::error_code& error)
{
    std::cout << " Write error = " << error.value() << std::endl;
    if ( !error.value() ){
        std::cout << "Write OK"<< std::endl;
	read_async();
    }
}

void client_test::add_query_message(std::string query)
{
    boost::lock_guard<boost::mutex> lock( m_mutex );
    m_output_messages.push_back(query);
    write_query_on_server();
}

void client_test::write_query_on_server()
{
    write_async(m_output_messages[0]);
    m_output_messages.erase( m_output_messages.begin() );
}



