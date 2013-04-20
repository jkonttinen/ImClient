#include "stdafx.hh"
#include "friendswindow.hh"

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  std::list<Glib::ustring> asd;
  asd.push_back("rambake");
  asd.push_back("Mr. Nobody's Friend");
  asd.push_back("diukkari");
  asd.push_back("Mr. Mianki");

  FriendsWindow window(asd);
  //Shows the window and returns when it is closed.
  Gtk::Main::run(window);

  return 0;
}

