#include "stdafx.hh"
#include "friendswindow.hh"

FriendsWindow::FriendsWindow()
    : m_Table(1, 10), m_Button_Close("Close"), m_Button_Chat("Chat")
{
    set_title("Friends");
    set_border_width(0);
    set_size_request(300, 300);

    m_ScrolledWindow.set_border_width(10);

    /* the policy is one of Gtk::POLICY AUTOMATIC, or Gtk::POLICY_ALWAYS.
     * Gtk::POLICY_AUTOMATIC will automatically decide whether you need
     * scrollbars, whereas Gtk::POLICY_ALWAYS will always leave the scrollbars
     * there.  The first one is the horizontal scrollbar, the second,
     * the vertical. */
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    add(vBox);
    vBox.pack_start(m_ScrolledWindow);
    vBox.pack_start(hBox, Gtk::PackOptions::PACK_SHRINK,1);

    /* set the spacing to 1 on x and 10 on y */
    m_Table.set_row_spacings(10);
    m_Table.set_col_spacings(1);

    /* pack the table into the scrolled window */
    m_ScrolledWindow.add(m_Table);

    /* this simply creates a grid of toggle buttons on the table
     * to demonstrate the scrolled window. */
    for(int i = 0; i < 1; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            char buffer[32];
            sprintf(buffer, "button (%d,%d)\n", i, j);
            Gtk::Button* pButton = Gtk::manage(new Gtk::ToggleButton(buffer));
            m_Table.attach(*pButton, i, i + 1, j, j + 1);
        }
    }

    /* Add a "close" button to the bottom of the dialog */
    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
            &FriendsWindow::on_button_close));
    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
            &FriendsWindow::on_button_chat));

    /* this makes it so the button is the default. */
    m_Button_Chat.set_can_default();

    //Gtk::Box* pBox = vBox.get_action_area();
    //if(vBox){
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

void FriendsWindow::on_button_close()
{
    hide();
}

void FriendsWindow::on_button_chat()
{

}
