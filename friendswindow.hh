#ifndef FRIENDSWINDOW_HH
#define FRIENDSWINDOW_HH

#include "stdafx.hh"

class FriendsWindow : public Gtk::Window
{
public:
    FriendsWindow(std::vector<Glib::ustring>);
    virtual ~FriendsWindow();

protected:
    //Signal handlers:
    void on_quit();
    void on_button_chat();
    void on_menu_nick();

    std::vector<Glib::ustring> names;

    //Child widgets:
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::Table m_Table;
    Gtk::Button m_Button_Close, m_Button_Chat;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

    Gtk::VBox vBox;
    Gtk::HBox hBox;
};

#endif //FRIENDSWINDOW_HH
