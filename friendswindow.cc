#include "stdafx.hh"
#include "friendswindow.hh"
#include "namedialog.hh"
#include "chatwindow.hh"
#include "connection.hh"

FriendsWindow::FriendsWindow(Connection* con)
    : m_Table(1,1), m_Button_Close("Close"), m_Button_Chat("Chat"),
    chatWin(NULL), connection(con)
{
    set_title("Friends");
    set_border_width(1);
    set_size_request(300, 300);
    m_ScrolledWindow.set_border_width(5);

    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    add(vBox);

    //menu stuff
    m_refActionGroup = Gtk::ActionGroup::create();

    m_refActionGroup->add(Gtk::Action::create("FileMenu", "_File"));
    m_refActionGroup->add(Gtk::Action::create("FileConnect", "Connect"),
                          sigc::mem_fun(*this, &FriendsWindow::on_menu_connect));
    m_refActionGroup->add(Gtk::Action::create("FileNick", "Nickname"),
                          sigc::mem_fun(*this, &FriendsWindow::on_menu_nick));
    m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                          sigc::mem_fun(*this, &FriendsWindow::on_quit));

    m_refUIManager = Gtk::UIManager::create();
    m_refUIManager->insert_action_group(m_refActionGroup);

    add_accel_group(m_refUIManager->get_accel_group());

    Glib::ustring ui_info =
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileConnect'/>"
        "      <menuitem action='FileNick'/>"
        "      <separator/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "   </menubar>"
        "</ui>";

    try {
        m_refUIManager->add_ui_from_string(ui_info);
    } catch(const Glib::Error& ex) {
        std::cerr << "building menus failed: " <<  ex.what();
    }

    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if(pMenubar)
        vBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);

    m_ScrolledWindow.add(m_Table);

    //set_namelist(names);

    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
            &FriendsWindow::on_quit));
    m_Button_Chat.signal_clicked().connect( sigc::mem_fun(*this,
                                            &FriendsWindow::on_button_chat));
    disp.connect(sigc::mem_fun(*this,&FriendsWindow::handle_msg));


    vBox.pack_start(m_ScrolledWindow);
    vBox.pack_start(hBox, Gtk::PACK_SHRINK,1);

    hBox.pack_start(m_Button_Chat);
    hBox.pack_start(m_Button_Close);

    m_Button_Chat.set_can_default();
    m_Button_Chat.grab_default();

    show_all_children();
}

FriendsWindow::~FriendsWindow()
{
    if (chatWin) delete chatWin;
    while(msg_queue.size() > 0) {
        delete msg_queue.front();
        msg_queue.pop();
    }
}

void FriendsWindow::set_namelist(const std::list<Glib::ustring>& names)
{
    for (auto it = buttons.begin(); it != buttons.end();) {
        m_Table.remove(*(it->second));
        auto help = it;
        help++;
        for (auto it1 = names.begin(); it1 != names.end();) {
            if (*it1 == (it->first)) break;
            it1++;
            if (it1 == names.end()) buttons.erase(it);
        }
        if (!names.size()) buttons.clear();
        it = help;
    }
    for (auto it = names.begin(); it != names.end(); it++) {
        if (buttons.find(*it) == buttons.end())
            buttons[*it] = Gtk::manage(new Gtk::ToggleButton(*it));
    }
    size_t i= 0;
    for (auto it = buttons.begin(); it != buttons.end(); it++) {
        it->second->set_size_request(0,50);
        m_Table.attach(*(it->second), 0, 1, i, i + 1);
        i++;
    }
    show_all_children();
}

void FriendsWindow::new_msg(const Message& msg)
{
    mutex.lock();
    msg_queue.push(new Message(msg.get_content(true)));
    mutex.unlock();
}

bool FriendsWindow::create_chatwin(const std::list<Glib::ustring>& names)
{
    if (chatWin && !chatWin->get_is_drawable()) {
        delete chatWin;
        chatWin = NULL;
    }
    if (!chatWin) {
        chatWin = new ChatWindow(names, nickName,connection);
        chatWin->show();
        return true;
    } else return false;
}

void FriendsWindow::handle_msg()
{
    mutex.lock();
    Message msg(msg_queue.front()->get_content(true));
    delete msg_queue.front();
    msg_queue.pop();
    switch(msg.get_type()) {
    case Message::CHAT_MESSAGE: {
        if (!chatWin->get_is_drawable()) delete chatWin;
        else chatWin->handle_msg(msg);
        break;
    }
    case Message::INVITE: {
        std::list<Glib::ustring> names;
        names.push_back("Group Chat");
        if (msg.get_name() != nickName && !create_chatwin(names)) chatWin->new_tab(names);
        chatWin->handle_msg(msg);
        break;
    }
    case Message::PART_CHAT:
    case Message::LIST_CHAT: {
        if (chatWin && chatWin->is_drawable()) chatWin->handle_msg(msg);
        break;
    }
    case Message::MESSAGE: {
        std::list<Glib::ustring> names;
        names.push_back(msg.get_name());
        create_chatwin(names);
        chatWin->handle_msg(msg);
        break;
    }
    case Message::LIST_ALL: {
        std::stringstream help;
        std::string str;
        std::list<Glib::ustring> names;
        help << msg;
        for (size_t i = 0;i < help.str().size();i++){
            if (help.str()[i] != '~') str += help.str()[i];
            else{
                names.push_back(str);
                str = std::string("");
            }
        }
        set_namelist(names);
        if (chatWin && chatWin->is_drawable()) chatWin->handle_msg(msg);
        break;
    }
    case Message::EXIT: {
        if (msg.get_content(false) == "")
            info_msg("No Connection", "The server is probably down.");
        else info_msg("No Connection", msg.get_content(false));
        std::list<Glib::ustring> names;
        set_namelist(names);
    }
    default:
        break;
    }
    mutex.unlock();
}

void FriendsWindow::info_msg(const Glib::ustring& primary, const Glib::ustring& secondary)
{
    Gtk::MessageDialog dl(*this, primary);
    dl.set_secondary_text(secondary);

    dl.run();
}

void FriendsWindow::on_quit()
{
    hide();
}

void FriendsWindow::on_button_chat()
{
    mutex.lock();
    std::list<Glib::ustring> names;
    for (auto it = buttons.begin(); it != buttons.end(); it++)
        if (it->second->get_active())
            names.push_back(it->second->get_label());

    if (!names.empty())
        if (!create_chatwin(names))
            chatWin->new_tab(names);

    mutex.unlock();
}

void FriendsWindow::on_menu_connect()
{
    if (nickName == "") on_menu_nick();
    if (!connection->is_connected())
        connection->connect(nickName,&disp,this);
}

void FriendsWindow::on_menu_nick()
{
    NameDialog nd(this);
    nd.run();
}

void FriendsWindow::set_nick(const Glib::ustring& name)
{
    mutex.lock();
    nickName = name;
    if (chatWin) chatWin->set_nick(name);
    mutex.unlock();
}

Glib::ustring FriendsWindow::get_nick()const
{
    return nickName;
}

