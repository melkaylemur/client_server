#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "client_test.h"

int main(int argc, char **argv) {
    client_test cl;
    cl.configure("127.0.0.1", "8084");
    while (true){
	    std::cout << "Input message: " << std::endl;
	    std::string str;
	    std::cin >> str;
	    cl.add_query_message(str);
    }
    boost::asio::io_service io;
    boost::asio::io_service::work wrkr(io);
    io.run();
    return 0;
}
