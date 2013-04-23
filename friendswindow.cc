#include "stdafx.hh"
#include "friendswindow.hh"
#include "namedialog.hh"
#include "chatwindow.hh"

FriendsWindow::FriendsWindow(std::list<Glib::ustring> names)
    : m_Table(names.size(),1), m_Button_Close("Close"), m_Button_Chat("Chat"), chatWin(NULL)
{
    set_title("Friends");
    set_border_width(1);
    set_size_request(300, 300);
    m_ScrolledWindow.set_border_width(5);

    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    add(vBox);

    //menu stuff
    m_refActionGroup = Gtk::ActionGroup::create();

    m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));
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

    set_namelist(names);

    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
            &FriendsWindow::on_quit));
    m_Button_Chat.signal_clicked().connect( sigc::mem_fun(*this,
                                            &FriendsWindow::on_button_chat));


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
    delete chatWin;
}

void FriendsWindow::set_namelist(std::list<Glib::ustring> names)
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

void FriendsWindow::on_quit()
{
    hide();
}

void FriendsWindow::on_button_chat()
{
    std::list<Glib::ustring> names;
    for (auto it = buttons.begin(); it != buttons.end(); it++)
        if (it->second->get_active())
            names.push_back(it->second->get_label());

    if (!names.empty()) {
        if (!(chatWin) || !(chatWin->get_is_drawable())) {
            chatWin = new ChatWindow(names);
            chatWin->show();
        } else chatWin->new_tab(names);
    }
}

void FriendsWindow::on_menu_nick()
{
    NameDialog nd;
    nd.run();
}

