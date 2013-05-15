#ifndef CHATWINDOW_HH
#define CHATWINDOW_HH

#include "messages.hh"
#include "connection.hh"

class Connection;

class ChatWindow : public Gtk::Window
{
public:
    ChatWindow(const std::list<Glib::ustring>&, const Glib::ustring&, Connection*);
    virtual ~ChatWindow();

    void new_tab(const std::list<Glib::ustring>&);
    void set_nick(const Glib::ustring&);
    void handle_msg(const Message&);
private:
    void on_send_clicked();
    void on_page_switched(GtkNotebookPage* page, guint page_num);
    void on_cross_clicked(Gtk::ScrolledWindow*);
    void on_close();

    void set_view_text(size_t page, const Glib::ustring& name, const Glib::ustring& msg);

    Gtk::Notebook nBook;

    Gtk::VBox vBox;
    Gtk::HBox hBox;

    Gtk::Entry writeEntry;
    Gtk::Button sendButton;

    std::vector<Gtk::TextView*> chatViews;
    std::vector<Glib::ustring> tags;

    Glib::ustring nickName;
    std::list<Glib::ustring> groupNames;
    std::vector<Gtk::Label*> chatters;

    Connection* connection;

    const Glib::ustring waiting;
};

#endif // CHATWINDOW_HH
