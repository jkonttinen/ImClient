#include "stdafx.hh"
#include "friendswindow.hh"

FriendsWindow::FriendsWindow()
: m_Table(10, 10),
  m_Button_Close("Close")
{
  set_title("Gtk::ScrolledWindow example");
  set_border_width(0);
  set_size_request(300, 300);

  m_ScrolledWindow.set_border_width(10);

  /* the policy is one of Gtk::POLICY AUTOMATIC, or Gtk::POLICY_ALWAYS.
   * Gtk::POLICY_AUTOMATIC will automatically decide whether you need
   * scrollbars, whereas Gtk::POLICY_ALWAYS will always leave the scrollbars
   * there.  The first one is the horizontal scrollbar, the second,
   * the vertical. */
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  get_vbox()->pack_start(m_ScrolledWindow);

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

  /* this makes it so the button is the default. */
  m_Button_Close.set_can_default();

  Gtk::Box* pBox = get_action_area();
  if(pBox)
    pBox->pack_start(m_Button_Close);

  /* This grabs this button to be the default button. Simply hitting
   * the "Enter" key will cause this button to activate. */
  m_Button_Close.grab_default();

  show_all_children();
}

FriendsWindow::~FriendsWindow()
{
}

void FriendsWindow::on_button_close()
{
  hide();
}
