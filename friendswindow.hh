#ifndef FRIENDSWINDOW_HH
#define FRIENDSWINDOW_HH

#include "chatwindow.hh"
#include "connection.hh"

class Connection;
class ChatWindow;

class FriendsWindow : public Gtk::Window
{
public:
    FriendsWindow(Connection*);
    virtual ~FriendsWindow();

    void set_namelist(const std::list<Glib::ustring>&);
    void set_nick(const Glib::ustring&);
    Glib::ustring get_nick()const;
    void handle_msg(const Message&);
private:
    //Signal handlers:
    void on_quit();
    void on_button_chat();
    void on_menu_nick();
    void on_menu_connect();

    std::map<Glib::ustring, Gtk::ToggleButton*> buttons;

    //Child widgets:
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::Table m_Table;
    Gtk::Button m_Button_Close, m_Button_Chat;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

    Gtk::VBox vBox;
    Gtk::HBox hBox;

    Glib::ustring nickName;

    ChatWindow *chatWin;

    Connection *connection;
    boost::mutex fMutex;
};

#endif //FRIENDSWINDOW_HH
