#include "stdafx.hh"
#include "chatwindow.hh"

ChatWindow::ChatWindow(const std::list<Glib::ustring>& names, const Glib::ustring& nickName,
                       Connection* con)
    : sendButton("Send"), nickName(nickName), connection(con)
{
    set_title("Chat");
    set_size_request(600,400);

    add(vBox);

    vBox.pack_start(nBook);
    vBox.pack_start(hBox, Gtk::PACK_SHRINK);

    hBox.pack_start(writeEntry);
    hBox.pack_start(sendButton, Gtk::PACK_SHRINK);

    new_tab(names);

    nBook.set_scrollable(true);

    sendButton.set_can_default();
    sendButton.grab_default();
    writeEntry.grab_focus();
    writeEntry.set_activates_default(true);

    sendButton.signal_clicked().connect(sigc::mem_fun(this,
                                        &ChatWindow::on_send_clicked));
    nBook.signal_switch_page().connect(sigc::mem_fun(*this,
                                       &ChatWindow::on_page_switched));
    show_all_children();
}

ChatWindow::~ChatWindow()
{
    for (auto it = chatViews.begin(); it != chatViews.end(); it++)
        delete (*it);
    hide();
}
void ChatWindow::new_tab(const std::list<Glib::ustring>& names)
{
    Gtk::ScrolledWindow* sw = Gtk::manage(new Gtk::ScrolledWindow);
    Gtk::HBox* textBox = Gtk::manage(new Gtk::HBox);
    Gtk::Label* nameLabel = Gtk::manage(new Gtk::Label);

    chatViews.push_back(new Gtk::TextView);
    if (names.size() == 1)
        tags.push_back(names.front());
    else {
        connection->send_to(Message("new",Message::INVITE,names.front()));
        tags.push_back("waiting");
    }

    sw->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    Glib::ustring str("");
    for (auto it = names.begin(); it != names.end(); it++) {
        auto help = it;
        help++;
        nameLabel->set_label(nameLabel->get_label() + (*it) + '\n');
        if (help != names.end()) str += (*it) + ", ";
        else str += (*it);
    }

    Gtk::HBox* hb = Gtk::manage(new Gtk::HBox);
    Gtk::Label* label = Gtk::manage(new Gtk::Label(str));

    Gtk::Button* button = Gtk::manage(new Gtk::Button);

    Gtk::Image* img = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE,
                                  Gtk::ICON_SIZE_MENU));
    button->add(*img);
    button->signal_clicked().connect(sigc::bind<Gtk::ScrolledWindow*>(sigc::mem_fun(*this,
                                     &ChatWindow::on_cross_clicked), sw) );
    hb->pack_start(*label);
    hb->pack_start(*button, Gtk::PACK_SHRINK);

    chatViews.back()->set_wrap_mode(Gtk::WRAP_WORD_CHAR);
    chatViews.back()->set_editable(false);

    nameLabel->set_size_request(120,0);
    nameLabel->set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP);

    sw->add(*textBox);
    textBox->pack_start(*(chatViews.back()));
    textBox->pack_start(*nameLabel, Gtk::PACK_SHRINK);

    nBook.append_page(*sw, *hb);

    hb->show_all_children();
    nBook.show_all_children();
}

void ChatWindow::on_send_clicked()
{
    if (!writeEntry.get_text_length()) return;
    set_view_text(nBook.get_current_page(),nickName, writeEntry.get_text());

    connection->send_to(Message(writeEntry.get_text(),Message::MESSAGE,
                                tags[nBook.get_current_page()]));
    writeEntry.set_text("");
}

void ChatWindow::on_page_switched(GtkNotebookPage* page, guint page_num)
{
    writeEntry.set_can_focus();
    writeEntry.grab_focus();
}

void ChatWindow::on_cross_clicked(Gtk::ScrolledWindow* sw)
{
    delete (chatViews[nBook.page_num(*sw)]);
    chatViews.erase(chatViews.begin() + nBook.page_num(*sw));
    tags.erase(tags.begin() + nBook.page_num(*sw));
    nBook.remove(*sw);
    if (!nBook.get_n_pages())
        hide();
}

void ChatWindow::set_nick(const Glib::ustring& name)
{
    nickName = name;
}

void ChatWindow::set_view_text(size_t page, const Glib::ustring& name, const Glib::ustring& msg)
{
    chatViews[page]->get_buffer()->set_text(chatViews[page]->get_buffer()->get_text()
                        + name + ": " + msg +"\n");
}

void ChatWindow::handle_msg(const Message& msg)
{
    switch (msg.get_type()) {
    case Message::MESSAGE: {
        size_t i;
        for (i = 0;i < tags.size();i++){
            if (msg.get_info() == tags[i]){
                set_view_text(i,msg.get_info(),msg.get_content(false));
                break;
            }
        }

        if (i == tags.size()){
            std::list<Glib::ustring> names;
            names.push_back(msg.get_info());
            new_tab(names);
            set_view_text(i, msg.get_info(), msg.get_content(false));
        }

        break;
    }
    default:
        break;
    }
}
