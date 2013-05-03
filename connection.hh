#ifndef CONNECTION_HH
#define CONNECTION_HH

class Connection
{
public:
    Connection(boost::asio::io_service&,
        boost::asio::ip::tcp::resolver::iterator&, const std::string& name);
    virtual ~Connection();
private:
    void on_connect();
    void listen();
    void close();

    boost::asio::io_service& io_service;
    boost::asio::ip::tcp::socket socket;
    boost::system::error_code error;
    boost::thread *t;

    std::string name;
};

#endif // CONNECTION_HH
