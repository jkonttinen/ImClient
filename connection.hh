#ifndef CONNECTION_HH
#define CONNECTION_HH


class Connection
{
public:
    Connection(boost::asio::io_service&,
        boost::asio::ip::tcp::resolver::iterator&, const Glib::ustring& name);
    virtual ~Connection();
private:
    boost::asio::io_service& io_service;
    boost::asio::ip::tcp::socket socket;
    Glib::ustring name;
};

#endif // CONNECTION_HH
