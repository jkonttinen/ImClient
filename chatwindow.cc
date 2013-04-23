#include "stdafx.hh"
#include "chatwindow.hh"

ChatWindow::ChatWindow(std::list<Glib::ustring> names)
    : sendButton("Send"), writeEntry()
{
    set_title("Chat");
    set_size_request(600,400);

    add(vBox);

    sendButton.signal_clicked().connect(sigc::mem_fun(*this,
                                        &ChatWindow::on_send_clicked) );

    vBox.pack_start(nBook);
    vBox.pack_start(hBox, Gtk::PACK_SHRINK);

    hBox.pack_start(writeEntry);
    hBox.pack_start(sendButton, Gtk::PACK_SHRINK);

    new_tab(names);

    sendButton.set_can_default();
    sendButton.grab_default();
    writeEntry.set_activates_default(true);

    show_all_children();
}

ChatWindow::~ChatWindow()
{
}
void ChatWindow::new_tab(std::list<Glib::ustring> names)
{
    Gtk::ScrolledWindow* sw = Gtk::manage(new Gtk::ScrolledWindow);

    Glib::ustring str("");
    for (auto it = names.begin(); it != names.end(); it++) {
        auto help = it;
        help++;
        if (help != names.end()) str += (*it) + ", ";
        else str += (*it);
    }

    Gtk::HBox* hb = Gtk::manage(new Gtk::HBox);
    Gtk::Label* label = Gtk::manage(new Gtk::Label(str));

    Gtk::Button* button = Gtk::manage(new Gtk::Button);
    //~ Gtk::Image* i = Gtk::manage(new Gtk::Image("littlecross.png"));
    Gtk::Image* img = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE,
                                  Gtk::ICON_SIZE_MENU));
    button->add(*img);
    button->signal_clicked().connect(sigc::bind<Gtk::ScrolledWindow*>(sigc::mem_fun(*this,
            &ChatWindow::on_cross_clicked), sw) );

    hb->pack_start(*label);
    hb->pack_start(*button, Gtk::PACK_SHRINK);

    nBook.append_page(*sw, *hb);
    hb->show_all_children();
    nBook.show_all_children();
}

void ChatWindow::on_send_clicked()
{
    std::cout <<writeEntry.get_text()<<std::endl;
}

void ChatWindow::on_cross_clicked(Gtk::ScrolledWindow* sw)
{
    nBook.remove(*sw);
    if (!nBook.get_n_pages())
        hide();
}
