#include "server.h"

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
	//std::cout << "Read OK size = " << read_bytes <<std::endl;
	boost::smatch res;
	boost::regex reg_file("FileName: *(.+?), FileSize: (\\w+), FileData: ");
    	std::string name, fsize;
        if(!(boost::regex_search(m_current_input_string, res, reg_file, boost::match_extra))){
	    const char * c = m_current_input_string.c_str();
	    outfile.write(c, m_current_input_string.size());
	    length_file += m_current_input_string.size();
	    std::cout << "Length = " << length_file <<std::endl;
            std::cout << "Not format "<< std::endl;
	    if (length_file == file_size){
	    	outfile.close();
	    }
        }
        else{
	    name = res[1];
            fsize = res[2];
            file_size = boost::lexical_cast<std::size_t>(res[2]);
	    outfile.open(name,std::ofstream::binary);
	    length_file = 0;
	    int header_length = 34 + name.size() + fsize.size();
    	    std::string data_new;
    	    for (int i = header_length; i < m_current_input_string.size(); i++){
        	data_new += m_current_input_string[i];
            }
	    const char * c = data_new.c_str();
	    outfile.write(c, data_new.size());
	    length_file += data_new.size();
	    std::cout << "Length = " << length_file <<std::endl;
        }	
	//while (length_file != file_size){
	    m_current_input_string = "";
	    read_async();
	//}
    }
}

void server::handle_write(const boost::system::error_code& error)
{
    std::cout << "Write result. error " << error.value() << std::endl;
    read_async();
}

