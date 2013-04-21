#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include "stdafx.hh"

class NameDialog : public Gtk::Dialog
{
    public:
        NameDialog();
        virtual ~NameDialog();
    protected:
        void on_button_clicked();
    private:
        Gtk::Entry entry;
};

#endif // NAMEDIALOG_H
