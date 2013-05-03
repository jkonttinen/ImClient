#include "stdafx.hh"
#include "connection.hh"
#include "messages.hh"

using boost::asio::ip::tcp;

Connection::Connection(boost::asio::io_service& io_service,
        tcp::resolver::iterator& iter, const std::string& name = "")
        : io_service(io_service), socket(io_service), t(NULL), name(name)
{
    boost::asio::connect(socket, iter, error);
    on_connect();
}

Connection::~Connection()
{
    if (t){
        if (t->joinable()){
            t->detach();
            t->interrupt();
            t->join();
        }
        delete t;
    }
}

void Connection::on_connect()
{
    std::stringstream ss;
    ss << name.length();

    if (!error){
        boost::asio::write(socket, boost::asio::buffer(ss.str().c_str(), 4));
        boost::asio::write(socket, boost::asio::buffer(name, name.length()));
        t = new boost::thread(&Connection::listen, this);
    }
    else close();
}

void Connection::close()
{
    socket.close();
}

void Connection::listen()
{
    char size[32], *buf;
    size_t rv = 1;
    while (rv){
        strcpy(size,"");

        rv = boost::asio::read(socket, boost::asio::buffer(size,32));
        buf = new char[atoi(size)+1];
        rv = boost::asio::read(socket, boost::asio::buffer(buf,atoi(size)));
        buf[atoi(size)] = '\0';
        std::cout << buf << std::endl;
        usleep(5);
        delete [] buf;
    }
    //t->join();
    close();
}
