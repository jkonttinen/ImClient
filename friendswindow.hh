#ifndef FRIENDSWINDOW_HH
#define FRIENDSWINDOW_HH

#include "stdafx.hh"

class FriendsWindow : public Gtk::Window
{
public:
    FriendsWindow();
    virtual ~FriendsWindow();

protected:
    //Signal handlers:
    void on_button_close();
    void on_button_chat();

    //Child widgets:
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::Table m_Table;
    Gtk::Button m_Button_Close, m_Button_Chat;

    Gtk::VBox vBox;
    Gtk::HBox hBox;
};

#endif //FRIENDSWINDOW_HH
