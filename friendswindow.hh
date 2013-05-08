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

    void set_nick(const Glib::ustring&);
    Glib::ustring get_nick()const;
    void new_msg(const Message&);
private:
    //Signal handlers:
    void on_quit();
    void on_button_chat();
    void on_menu_nick();
    void on_menu_connect();
    void set_namelist(const std::list<Glib::ustring>&);
    void handle_msg();

    std::map<Glib::ustring, Gtk::ToggleButton*> buttons;
    std::queue<Message*> msg_queue;
    //Child widgets:
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::Table m_Table;
    Gtk::Button m_Button_Close, m_Button_Chat;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

    Glib::Dispatcher disp;

    Gtk::VBox vBox;
    Gtk::HBox hBox;

    Glib::ustring nickName;

    ChatWindow *chatWin;
    Connection *connection;
    boost::mutex mutex;
};

#endif //FRIENDSWINDOW_HH
