#ifndef NAMEDIALOG_HH
#define NAMEDIALOG_HH

class NameDialog : public Gtk::Dialog
{
public:
    NameDialog();
    virtual ~NameDialog();
private:
    void on_button_clicked();

    Gtk::Entry entry;
};

#endif // NAMEDIALOG_HH
