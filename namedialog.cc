#include "stdafx.hh"
#include "namedialog.hh"

NameDialog::NameDialog()
{
    set_title("Choose nickname");

    Gtk::VBox *vBox = get_vbox();
    Gtk::Button *button = new Gtk::Button(Gtk::Stock::OK);

    vBox->pack_start(entry);
    vBox->pack_start(*button);

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
// TODO (Juhana#1#): Tell somebody what the name actually is.

    std::cout << entry.get_text() << std::endl;
    hide();
}
