#ifndef CONNECTION_HH
#define CONNECTION_HH

#include "messages.hh"
#include "friendswindow.hh"

class FriendsWindow;

class Connection
{
public:
    Connection(boost::asio::io_service&, boost::asio::ip::tcp::resolver::iterator&);
    virtual ~Connection();

    void connect(const Glib::ustring&,Glib::Dispatcher*, FriendsWindow*);
    void send_to(const Message&);
    bool is_connected()const;
private:
    void listen();

    boost::asio::io_service& io_service;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver::iterator& iter;
    boost::system::error_code error;
    boost::thread t;

    Glib::Dispatcher* disp;
    Glib::ustring nickName;
    bool connected;

    FriendsWindow *fwin;
};

#endif // CONNECTION_HH
