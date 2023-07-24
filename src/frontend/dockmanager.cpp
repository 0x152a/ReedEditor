#include "dockmanager.h"
#include "FL/Enumerations.H"
#include "mainwindow.h"

#include <FL/Fl_Tile.H>
#include <FL/Fl_Box.H>


DockManager::DockManager(MainWindow *parent) :
    Fl_Tile(parent->x(), parent->y(), parent->w(), parent->h()),
    parent(parent)
{
    this->end();

    Fl_Box *b1 = new Fl_Box(0, 0, 150, 300, "1");
    b1->color(9);
    b1->box(FL_DOWN_BOX);
    b1->align(FL_ALIGN_CLIP);
    Fl_Box *b2 = new Fl_Box(150, 0, 150, 300, "2");
    b2->color(19);
    b2->box(FL_DOWN_BOX);
    b2->align(FL_ALIGN_CLIP);
    this->show();
    this->add(b1);
    this->add(b2);

}