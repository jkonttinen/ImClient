#ifndef CHATWINDOW_HH
#define CHATWINDOW_HH

class ChatWindow : public Gtk::Window
{
public:
    ChatWindow(const std::list<Glib::ustring>&, const Glib::ustring&);
    virtual ~ChatWindow();

    void new_tab(const std::list<Glib::ustring>&);
    void set_nick(const Glib::ustring&);
private:
    void on_send_clicked();
    void on_page_switched(GtkNotebookPage* page, guint page_num);
    void on_cross_clicked(Gtk::ScrolledWindow*);

    Gtk::Notebook nBook;

    Gtk::VBox vBox;
    Gtk::HBox hBox;

    Gtk::Entry writeEntry;
    Gtk::Button sendButton;

    std::vector<Gtk::TextView*> chatViews;

    Glib::ustring nickName;
};

#endif // CHATWINDOW_HH
