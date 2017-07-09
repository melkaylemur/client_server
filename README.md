# client_server

Client:
g++ -g main.cpp client_test.h client_test.cpp -o client -lboost_system -lboost_filesystem -std=c++11 -lpthread -lboost_thread

Server:
g++ -g main.cpp server.h server.cpp -o server -lboost_system -lboost_filesystem -std=c++11 -lpthread -lboost_thread
