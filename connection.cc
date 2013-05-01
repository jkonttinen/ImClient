#include "stdafx.hh"
#include "connection.hh"
#include "messages.hh"

using boost::asio::ip::tcp;

Connection::Connection(boost::asio::io_service& io_service,
        tcp::resolver::iterator& iter, const Glib::ustring& name = "")
        : io_service(io_service), socket(io_service), name(name)
{
}

Connection::~Connection()
{
    //dtor
}
