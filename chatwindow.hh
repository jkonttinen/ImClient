#ifndef CHATWINDOW_HH
#define CHATWINDOW_HH

class ChatWindow : public Gtk::Window
{
public:
    ChatWindow(std::list<Glib::ustring>);
    virtual ~ChatWindow();

    void new_tab(std::list<Glib::ustring>);
private:
    void on_send_clicked();
    void on_cross_clicked(Gtk::ScrolledWindow*);

    Gtk::Notebook nBook;

    Gtk::VBox vBox;
    Gtk::HBox hBox;

    Gtk::Button sendButton;
    Gtk::Entry writeEntry;
};

#endif // CHATWINDOW_HH
