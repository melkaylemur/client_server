#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "client_test.h"

int main(int argc, char **argv) {
    client_test cl;
    cl.configure("127.0.0.1", "8084");
    while (true){
	    std::cout << "Input name file: " << std::endl;
	    std::string file_path;
	    std::cin >> file_path;
	    std::ifstream infile (file_path,std::ifstream::binary);

	    infile.seekg (0,infile.end);
	    long size = infile.tellg();
	    infile.seekg (0);

	    char* buffer = new char[size];
	    infile.read (buffer,size);
	    std::string data(buffer, size);
	    
	    std::string file_name;
	    file_name = "FileName: " + file_path + ", ";

	    uintmax_t f_size;
    	    f_size = size;
   	    std::string file_size;
    	    file_size = "FileSize: " + boost::lexical_cast<std::string>(f_size) +", ";
	    
	    std::string file_data;
	    file_data = "FileData: " + data;
	    
	    std::string str;
	    str = file_name + file_size + file_data;

	    cl.add_query_message(str);
	    delete [] buffer;
	    infile.close();
    }
    boost::asio::io_service io;
    boost::asio::io_service::work wrkr(io);
    io.run();
    return 0;
}
