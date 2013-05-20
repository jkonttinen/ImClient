#include "stdafx.hh"
#include "chatwindow.hh"

ChatWindow::ChatWindow(const std::list<Glib::ustring>& names, const Glib::ustring& nickName,
                       Connection* con)
    : sendButton("Send"), nickName(nickName), connection(con), waiting("waiting")
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
    signal_hide().connect(sigc::mem_fun(*this,&ChatWindow::on_close));

    show_all_children();
}

ChatWindow::~ChatWindow()
{
    size_t i = 0;
    for (auto it = chatViews.begin(); it != chatViews.end(); it++) {
        connection->send_to(Message(Message::PART_CHAT,nickName,"",tags[i++]));
        delete (*it);
    }
    chatViews.clear();
    for (auto it = chatters.begin(); it != chatters.end(); it++)
        delete *it;
    //hide();
}
void ChatWindow::new_tab(const std::list<Glib::ustring>& names)
{
    Gtk::ScrolledWindow* sw = Gtk::manage(new Gtk::ScrolledWindow);
    Gtk::HBox* textBox = Gtk::manage(new Gtk::HBox);

    chatters.push_back(new Gtk::Label);
    chatViews.push_back(new Gtk::TextView);

    sw->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    if (names.size() == 1)
            tags.push_back(names.front());
    else {
        connection->send_to(Message(Message::INVITE,names.front(),"new"));
        tags.push_back(waiting);
        groupNames = names;
    }

    Glib::ustring str("");
    for (auto it = names.begin(); it != names.end(); it++) {
        auto help = it;
        help++;
        chatters.back()->set_label(chatters.back()->get_label() + (*it) + '\n');
        if (help != names.end()) str += (*it) + ", ";
        else str += (*it);
    }

    Gtk::HBox* hb = Gtk::manage(new Gtk::HBox);
    if (names.size() != 1) str = "Group Chat";
    Gtk::Label* label = Gtk::manage(new Gtk::Label(str));

    Gtk::Button* button = Gtk::manage(new Gtk::Button);

    Gtk::Image* img = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE,
                                  Gtk::ICON_SIZE_MENU));
    button->add(*img);
    button->signal_clicked().connect(sigc::bind<Gtk::HBox*>(sigc::mem_fun(*this,
                                     &ChatWindow::on_cross_clicked), textBox) );
    hb->pack_start(*label);
    hb->pack_start(*button, Gtk::PACK_SHRINK);

    chatViews.back()->set_wrap_mode(Gtk::WRAP_WORD_CHAR);
    chatViews.back()->set_editable(false);

    chatters.back()->set_size_request(120,0);
    chatters.back()->set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP);

    sw->add(*(chatViews.back()));
    textBox->pack_start(*sw);
    textBox->pack_start(*chatters.back(), Gtk::PACK_SHRINK);

    nBook.append_page(*textBox, *hb);

    hb->show_all_children();
    nBook.show_all_children();
}

void ChatWindow::on_close()
{
    size_t i = 0;
    for (auto it = chatViews.begin(); it != chatViews.end(); it++) {
        connection->send_to(Message(Message::PART_CHAT,nickName,"",tags[i++]));
        delete (*it);
    }
    chatViews.clear();
}

void ChatWindow::on_send_clicked()
{
    if (!writeEntry.get_text_length()) return;
    set_view_text(nBook.get_current_page(),nickName, writeEntry.get_text());

    if (tags[nBook.get_current_page()].find("Chat") == std::string::npos)
        connection->send_to(Message(Message::MESSAGE, tags[nBook.get_current_page()],
                                    writeEntry.get_text()));
    else connection->send_to(Message(Message::CHAT_MESSAGE, nickName,
                                         writeEntry.get_text(),tags[nBook.get_current_page()]));
    writeEntry.set_text("");
}

void ChatWindow::on_page_switched(GtkNotebookPage* page, guint page_num)
{
    writeEntry.set_can_focus();
    writeEntry.grab_focus();
}

void ChatWindow::on_cross_clicked(Gtk::HBox* hb)
{
    delete (chatViews[nBook.page_num(*hb)]);
    chatViews.erase(chatViews.begin() + nBook.page_num(*hb));
    connection->send_to(Message(Message::PART_CHAT, nickName,"",tags[nBook.page_num(*hb)]));
    tags.erase(tags.begin() + nBook.page_num(*hb));
    nBook.remove(*hb);
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
    Gtk::TextIter endIt = chatViews[page]->get_buffer()->end();
    chatViews[page]->scroll_to(endIt);
}

void ChatWindow::handle_msg(const Message& msg)
{
    Glib::ustring tab_name = "";
    switch (msg.get_type()) {
    case Message::CHAT_MESSAGE:
        tab_name = msg.get_chat();
    case Message::MESSAGE: {
        if (tab_name == "") tab_name = msg.get_name();
        size_t i;
        for (i = 0; i < tags.size(); i++) {
            if (tab_name == tags[i]) {
                set_view_text(i,msg.get_name(),msg.get_content(false));
                break;
            }
        }
        if (i == tags.size()) {
            std::list<Glib::ustring> names;
            names.push_back(msg.get_name());
            new_tab(names);
            set_view_text(i, msg.get_name(), msg.get_content(false));
        }

        break;
    }

    case Message::INVITE: {
        if (msg.get_name() == nickName) {
            for (size_t i = 0; i < tags.size(); i++) {
                if (tags[i] == waiting) {
                    tags[i] = msg.get_chat();
                    for (auto it = groupNames.begin(); it != groupNames.end(); it++)
                        if (it != groupNames.begin())
                            connection->send_to(Message(Message::INVITE, *it, nickName, msg.get_chat()));
                }
            }

        }
        else tags.back() = msg.get_chat();

        connection->send_to(Message(Message::LIST_CHAT,nickName,"",msg.get_chat()));
        break;
    }
    case Message::LIST_CHAT: {
        for (size_t i = 0; i < tags.size(); i++) {
            if (tags[i] == msg.get_chat()) {
                Glib::ustring str = msg.get_content(false), help;
                chatters[i]->set_label("");

                for (size_t j = 0; j < str.size(); j++) {
                    if (str[j] != '~') help += str[j];
                    else {
                        (chatters[i])->set_label((chatters[i])->get_label() + help + '\n');
                        help = "";
                    }
                }
                break;
            }
        }
        break;
    }
    case Message::PART_CHAT: {
        if (msg.get_name() != nickName) {
            for (size_t i = 0; i < tags.size(); i++)
                if (tags[i] == msg.get_chat()) {
                    Glib::ustring str = ":* " + msg.get_name() + " " + msg.get_content(false) + " *";
                    set_view_text(i, str, "");
                }

            connection->send_to(Message(Message::LIST_CHAT,nickName,"",msg.get_chat()));
        }
        break;
    }
    default:
        break;
    }
}
