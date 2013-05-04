#include "stdafx.hh"
#include "connection.hh"
#include "messages.hh"

using boost::asio::ip::tcp;

Connection::Connection(boost::asio::io_service& io_service,
        tcp::resolver::iterator& iter)
        : io_service(io_service), socket(io_service), iter(iter), connected(false)
{
}

Connection::~Connection()
{
}

void Connection::connect(const Glib::ustring& name)
{
    nickName = name;
    std::stringstream ss;
    ss << nickName.length();

    boost::asio::connect(socket, iter, error);

    if (!error){
        boost::asio::write(socket, boost::asio::buffer(ss.str().c_str(), 4));
        boost::asio::write(socket, boost::asio::buffer(nickName.c_str(), nickName.length()));
        t = boost::thread(&Connection::listen, this);
    }
    connected = true;
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
}

bool Connection::is_connected() const
{
    return connected;
}
