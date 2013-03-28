#ifndef FRIENDSWINDOW_HH
#define FRIENDSWINDOW_HH

#include "stdafx.hh"

class FriendsWindow : public Gtk::Dialog
{
public:
  FriendsWindow();
  virtual ~FriendsWindow();

protected:
  //Signal handlers:
  void on_button_close();

  //Child widgets:
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::Table m_Table;
  Gtk::Button m_Button_Close;
};

#endif //FRIENDSWINDOW_HH
