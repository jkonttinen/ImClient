#include "stdafx.hh"
#include "namedialog.hh"

NameDialog::NameDialog(FriendsWindow* fw) : parent(fw)
{
    set_title("Choose nickname");

    Gtk::Button *button = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));

    get_vbox()->pack_start(entry);
    get_vbox()->pack_start(*button);

    button->set_can_default();
    button->grab_default();

    entry.set_activates_default(true);
    button->signal_clicked().connect(sigc::mem_fun(*this,
              &NameDialog::on_button_clicked) );

    show_all_children();
}

NameDialog::~NameDialog()
{
}

void NameDialog::on_button_clicked()
{
    parent->set_nick(entry.get_text());
    hide();
}
