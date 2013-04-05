#include "stdafx.hh"
#include "friendswindow.hh"

FriendsWindow::FriendsWindow(std::vector<Glib::ustring> names)
    : names(names), m_Table(names.size(),1), m_Button_Close("Close"), m_Button_Chat("Chat")
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

    try
    {
        m_refUIManager->add_ui_from_string(ui_info);
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "building menus failed: " <<  ex.what();
    }

    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if(pMenubar)
        vBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);

//    /* set the spacing to 1 on x and 10 on y */
//    m_Table.set_row_spacings(10);
//    m_Table.set_col_spacings(1);

    /* pack the table into the scrolled window */
    m_ScrolledWindow.add(m_Table);

    for(size_t i = 0; i < names.size(); i++)
    {
        Gtk::Button* pButton = Gtk::manage(new Gtk::ToggleButton(names[i]));
        m_Table.attach(*pButton, 0, 1, i, i + 1);
    }


    /* Add a "close" button to the bottom of the dialog */
    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
            &FriendsWindow::on_quit));
    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
            &FriendsWindow::on_button_chat));

    /* this makes it so the button is the default. */
    m_Button_Chat.set_can_default();

    vBox.pack_start(m_ScrolledWindow);
    vBox.pack_start(hBox, Gtk::PackOptions::PACK_SHRINK,1);

    hBox.pack_start(m_Button_Chat);
    hBox.pack_start(m_Button_Close);


    /* This grabs this button to be the default button. Simply hitting
     * the "Enter" key will cause this button to activate. */
    m_Button_Chat.grab_default();

    show_all_children();
}

FriendsWindow::~FriendsWindow()
{
}

void FriendsWindow::on_quit()
{
    hide();
}

void FriendsWindow::on_button_chat()
{

}

void FriendsWindow::on_menu_nick()
{

}

