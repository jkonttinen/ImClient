#ifndef CONNECTION_HH
#define CONNECTION_HH

#include "messages.hh"

class Connection
{
public:
    Connection(boost::asio::io_service&, boost::asio::ip::tcp::resolver::iterator&);
    virtual ~Connection();

    void connect(const Glib::ustring&);
    void send_to(const Message&);
    bool is_connected()const;
private:
    void listen();

    boost::asio::io_service& io_service;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver::iterator& iter;
    boost::system::error_code error;
    boost::thread t;
    boost::mutex cMutex;

    Glib::ustring nickName;
    bool connected;
};

#endif // CONNECTION_HH
