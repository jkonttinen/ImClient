#include "stdafx.hh"
#include "friendswindow.hh"

int main(int argc, char *argv[])
{
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("mip4.kyla.fi", "12345");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    Connection con(io_service, iterator);

    Gtk::Main kit(argc, argv);

    std::list<Glib::ustring> asd;
    asd.push_back("rambake");
    asd.push_back("damb");
    asd.push_back("Jusku");
    asd.push_back("Mr. Nobody's Friend");
    asd.push_back("diukkari");
    asd.push_back("Mr. Mianki");

    FriendsWindow window(asd, &con);
    //Shows the window and returns when it is closed.
    Gtk::Main::run(window);

    return 0;
}

