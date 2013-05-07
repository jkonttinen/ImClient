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
    if (socket.is_open()){
        //socket.shutdown(tcp::socket::shutdown_both);
        socket.close();
        t.detach();
        t.interrupt();
        t.join();
    }
    else t.join();
}

void Connection::connect(const Glib::ustring& name, FriendsWindow* fwin)
{
    this->fwin = fwin;
    nickName = name;
    std::stringstream ss;
    ss << nickName.length();

    boost::asio::connect(socket, iter, error);

    if (!error){
        connected = true;
        boost::asio::write(socket, boost::asio::buffer(ss.str().c_str(), 4));
        boost::asio::write(socket, boost::asio::buffer(nickName.c_str(), nickName.length()));
        t = boost::thread(&Connection::listen, this);
    }
}

void Connection::send_to(const Message& msg)
{
    if (connected){
        size_t rv;
        std::stringstream os,help;
        os << msg.get_content(true);
        help << os.str().size();

        rv = boost::asio::write(socket, boost::asio::buffer(help.str().c_str(), 32));
        if (!rv) std::cout <<"Error while trying to send" << std::endl;
        rv = boost::asio::write(socket, boost::asio::buffer(os.str().c_str(), os.str().size()));
        if (!rv) std::cout <<"Error while trying to send" << std::endl;
    }
}

void Connection::listen()
{
    char size[32], *buf;
    size_t rv = 1;
    while (rv){
        strcpy(size,"");
        rv = boost::asio::read(socket, boost::asio::buffer(size,32),error);
        buf = new char[atoi(size)+1];
        rv = boost::asio::read(socket, boost::asio::buffer(buf,atoi(size)),error);
        buf[atoi(size)] = '\0';
        Message msg(buf);
        std::cout << buf << std::endl;
        if (msg.get_type() == Message::EXIT) break;
        fwin->handle_msg(msg);
        Sleep(5);
        delete [] buf;
    }
    socket.shutdown(tcp::socket::shutdown_both);
    socket.close();
    connected = false;
}

bool Connection::is_connected() const
{
    return connected;
}
