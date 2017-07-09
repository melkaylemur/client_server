#include <iostream>
#include <boost/asio.hpp>

#include "server.h"

int main(int argc, char **argv) {
    server sv;
    sv.configure();
    sv.start();
    boost::asio::io_service io;
    boost::asio::io_service::work wrkr(io);
    io.run();
    return 0;
}
