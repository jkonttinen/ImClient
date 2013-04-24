#ifndef NAMEDIALOG_HH
#define NAMEDIALOG_HH

#include "friendswindow.hh"

class NameDialog : public Gtk::Dialog
{
public:
    NameDialog(FriendsWindow*);
    virtual ~NameDialog();
private:
    void on_button_clicked();
    FriendsWindow* parent;

    Gtk::Entry entry;
};

#endif // NAMEDIALOG_HH
