#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/filename.H>
#include <FL/fl_ask.H>
#include <FL/x.H>  // for fl_open_callback
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveCB();
void saveasCB();
void find2CB(Fl_Widget*, void*);
void replallCB(Fl_Widget*, void*);
void replace2CB(Fl_Widget*, void*);
void replcAnCB(Fl_Widget*, void*);

class EditorWindow : public Fl_Double_Window
{
  public:
    EditorWindow(int w, int h, const char* t);
    ~EditorWindow();

    Fl_Window* replace_dlg;
    Fl_Input* replace_find;
    Fl_Input* replace_with;
    Fl_Button* replace_all;
    Fl_Return_Button* replace_next;
    Fl_Button* replace_cancel;

    int wrap_mode;
    int line_numbers;

    Fl_Text_Editor* editor;
    char search[256];
};

int checkSave(void);
Fl_Window* newView();
void loadFile(const char* newfile, int ipos);
void saveFile(const char* newfile);
void copyCB(Fl_Widget*, void* v);
void cutCB(Fl_Widget*, void* v);
void deleteCB(Fl_Widget*, void*);

void linenumbersCB(Fl_Widget* w, void* v);
void wordwrapCB(Fl_Widget* w, void* v);
void findCB(Fl_Widget* w, void* v);
void find2CB(Fl_Widget* w, void* v);
void setTitle(Fl_Window* w);
void changedCB(int, int nInserted, int nDeleted, int, const char*, void* v);
void newCB(Fl_Widget*, void*);
void openCB(Fl_Widget*, void*);
void insertCB(Fl_Widget*, void* v);
void pasteCB(Fl_Widget*, void* v);
void closeCB(Fl_Widget*, void* v);
void quitCB(Fl_Widget*, void*);
void replaceCB(Fl_Widget*, void* v);
void replace2CB(Fl_Widget*, void* v);
void replaceAllCB(Fl_Widget*, void* v);
void replcAnCB(Fl_Widget*, void* v);
void saveCB();
void saveAsCB();
void viewCB(Fl_Widget*, void*);
void loadFileCB(const char* fname);
