#include "stdafx.hh"
#include "friendswindow.hh"

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  FriendsWindow window;
  //Shows the window and returns when it is closed.
  Gtk::Main::run(window);

  return 0;
}

