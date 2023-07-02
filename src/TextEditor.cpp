#include "TextEditor.h"

int G_changed = 0;
int G_loading = 0;
int G_num_windows = 0;
Fl_Text_Buffer *G_text_buffer = new Fl_Text_Buffer;
char G_filename[FL_PATH_MAX] = "";
char G_title[FL_PATH_MAX];

const int LINE_NUM_WIDTH = 75;
const int TEXT_SIZE = 14;

EditorWindow::EditorWindow(int w, int h, const char *t)
    : Fl_Double_Window(w, h, t) {
  replace_dlg = new Fl_Window(300, 105, "Replace");
  replace_find = new Fl_Input(80, 10, 210, 25, "Find:");
  replace_find->align(FL_ALIGN_LEFT);

  replace_with = new Fl_Input(80, 40, 210, 25, "Replace:");
  replace_with->align(FL_ALIGN_LEFT);

  replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
  replace_all->callback((Fl_Callback *)replaceAllCB, this);

  replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
  replace_next->callback((Fl_Callback *)replace2CB, this);

  replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
  replace_cancel->callback((Fl_Callback *)replcAnCB, this);
  replace_dlg->end();
  replace_dlg->set_non_modal();
  editor = 0;
  *search = (char)0;
  wrap_mode = 0;
  line_numbers = 0;
}

EditorWindow::~EditorWindow() { delete replace_dlg; }

int checkSave(void) {
  if (!G_changed)
    return 1;

  int r = fl_choice("The current file has not been saved.\n"
                    "Would you like to save it now?",
                    "Cancel", "Save", "Don't Save");

  if (r == 1) {
    saveCB(); // Save the file...
    return !G_changed;
  }

  return (r == 2) ? 1 : 0;
}

void loadFile(const char *newfile, int ipos) {
  G_loading = 1;
  int insert = (ipos != -1);
  G_changed = insert;
  if (!insert)
    strcpy(G_filename, "");
  int r;
  if (!insert)
    r = G_text_buffer->loadfile(newfile);
  else
    r = G_text_buffer->insertfile(newfile, ipos);
  G_changed = G_changed || G_text_buffer->input_file_was_transcoded;
  if (r)
    fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
  else if (!insert)
    strcpy(G_filename, newfile);
  G_loading = 0;
  G_text_buffer->call_modify_callbacks();
}

void saveFile(const char *newfile) {
  if (G_text_buffer->savefile(newfile))
    fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
  else
    strcpy(G_filename, newfile);
  G_changed = 0;
  G_text_buffer->call_modify_callbacks();
}

void copyCB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  Fl_Text_Editor::kf_copy(0, e->editor);
}

void cutCB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  Fl_Text_Editor::kf_cut(0, e->editor);
}

void deleteCB(Fl_Widget *, void *) { G_text_buffer->remove_selection(); }

void lineNumbersCB(Fl_Widget *w, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  Fl_Menu_Bar *m = (Fl_Menu_Bar *)w;
  const Fl_Menu_Item *i = m->mvalue();
  if (i->value()) {
    e->editor->linenumber_width(LINE_NUM_WIDTH); // enable
    e->editor->linenumber_size(e->editor->textsize());
  } else {
    e->editor->linenumber_width(0); // disable
  }
  e->line_numbers = (i->value() ? 1 : 0);
  e->redraw();
}

void wordWrapCB(Fl_Widget *w, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  Fl_Menu_Bar *m = (Fl_Menu_Bar *)w;
  const Fl_Menu_Item *i = m->mvalue();
  if (i->value())
    e->editor->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  else
    e->editor->wrap_mode(Fl_Text_Display::WRAP_NONE, 0);
  e->wrap_mode = (i->value() ? 1 : 0);
  e->redraw();
}

void findCB(Fl_Widget *w, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  const char *val;

  val = fl_input("Search String:", e->search);
  if (val != NULL) {
    strcpy(e->search, val);
    find2CB(w, v);
  }
}

void find2CB(Fl_Widget *w, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  if (e->search[0] == '\0') {
    findCB(w, v);
    return;
  }

  int pos = e->editor->insert_position();
  int found = G_text_buffer->search_forward(pos, e->search, &pos);
  if (found) {
    G_text_buffer->select(pos, pos + strlen(e->search));
    e->editor->insert_position(pos + strlen(e->search));
    e->editor->show_insert_position();
  } else
    fl_alert("No occurrences of \'%s\' found!", e->search);
}

void setTitle(Fl_Window *w) {
  if (G_filename[0] == '\0')
    strcpy(G_title, "Untitled");
  else {
    char *slash;
    slash = strrchr(G_filename, '/');
#ifdef WIN32
    if (slash == NULL)
      slash = strrchr(filename, '\\');
#endif
    if (slash != NULL)
      strcpy(G_title, slash + 1);
    else
      strcpy(G_title, G_filename);
  }

  if (G_changed)
    strcat(G_title, " (modified)");

  w->label(G_title);
}

void changedCB(int, int nInserted, int nDeleted, int, const char *, void *v) {
  if ((nInserted || nDeleted) && !G_loading)
    G_changed = 1;
  EditorWindow *w = (EditorWindow *)v;
  setTitle(w);
  if (G_loading)
    w->editor->show_insert_position();
}

void newCB(Fl_Widget *, void *) {
  if (!checkSave())
    return;

  G_filename[0] = '\0';
  G_text_buffer->select(0, G_text_buffer->length());
  G_text_buffer->remove_selection();
  G_changed = 0;
  G_text_buffer->call_modify_callbacks();
}

void openCB(Fl_Widget *, void *) {
  if (!checkSave())
    return;
  Fl_Native_File_Chooser fnfc;
  fnfc.title("Open file");
  fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
  if (fnfc.show())
    return;
  loadFile(fnfc.filename(), -1);
}

void insertCB(Fl_Widget *, void *v) {
  Fl_Native_File_Chooser fnfc;
  fnfc.title("Insert file");
  fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
  if (fnfc.show())
    return;
  EditorWindow *w = (EditorWindow *)v;
  loadFile(fnfc.filename(), w->editor->insert_position());
}

void pasteCB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  Fl_Text_Editor::kf_paste(0, e->editor);
}

void closeCB(Fl_Widget *, void *v) {
  EditorWindow *w = (EditorWindow *)v;

  if (G_num_windows == 1) {
    if (!checkSave())
      return;
  }

  w->hide();
  w->editor->buffer(0);
  G_text_buffer->remove_modify_callback(changedCB, w);
  Fl::delete_widget(w);

  G_num_windows--;
  if (!G_num_windows)
    exit(0);
}

void quitCB(Fl_Widget *, void *) {
  if (G_changed && !checkSave())
    return;

  exit(0);
}

void replaceCB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  e->replace_dlg->show();
}

void replace2CB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  const char *find = e->replace_find->value();
  const char *replace = e->replace_with->value();

  if (find[0] == '\0') {
    // Search string is blank
    e->replace_dlg->show();
    return;
  }

  e->replace_dlg->hide();

  int pos = e->editor->insert_position();
  int found = G_text_buffer->search_forward(pos, find, &pos);

  if (found) {
    // Found a match
    G_text_buffer->select(pos, pos + strlen(find));
    G_text_buffer->remove_selection();
    G_text_buffer->insert(pos, replace);
    G_text_buffer->select(pos, pos + strlen(replace));
    e->editor->insert_position(pos + strlen(replace));
    e->editor->show_insert_position();
  } else
    fl_alert("No occurrences of \'%s\' found!", find);
}

void replaceAllCB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  const char *find = e->replace_find->value();
  const char *replace = e->replace_with->value();

  find = e->replace_find->value();
  if (find[0] == '\0') {
    // Search string is blank
    e->replace_dlg->show();
    return;
  }

  e->replace_dlg->hide();

  e->editor->insert_position(0);
  int times = 0;

  for (int found = 1; found;) {
    int pos = e->editor->insert_position();
    found = G_text_buffer->search_forward(pos, find, &pos);

    if (found) {
      // Found a match
      G_text_buffer->select(pos, pos + strlen(find));
      G_text_buffer->remove_selection();
      G_text_buffer->insert(pos, replace);
      e->editor->insert_position(pos + strlen(replace));
      e->editor->show_insert_position();
      times++;
    }
  }

  if (times)
    fl_message("Replaced %d occurrences.", times);
  else
    fl_alert("No occurrences of \'%s\' found!", find);
}

void replcAnCB(Fl_Widget *, void *v) {
  EditorWindow *e = (EditorWindow *)v;
  e->replace_dlg->hide();
}

void saveCB() {
  if (G_filename[0] == '\0') {
    // No filename - get one!
    saveAsCB();
    return;
  } else
    saveFile(G_filename);
}

void saveAsCB() {
  Fl_Native_File_Chooser fnfc;
  fnfc.title("Save File As?");
  fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  if (fnfc.show())
    return;
  saveFile(fnfc.filename());
}

Fl_Window *newView();

void viewCB(Fl_Widget *, void *) {
  Fl_Window *w = newView();
  w->show();
}

Fl_Menu_Item MENU_ITEMS[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
    {"&New File", 0, (Fl_Callback *)newCB},
    {"&Open File...", FL_COMMAND + 'o', (Fl_Callback *)openCB},
    {"&Insert File...", FL_COMMAND + 'i', (Fl_Callback *)insertCB, 0,
     FL_MENU_DIVIDER},
    {"&Save File", FL_COMMAND + 's', (Fl_Callback *)saveCB},
    {"Save File &As...", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback *)saveAsCB,
     0, FL_MENU_DIVIDER},
    {"New &View",
     FL_ALT
#ifdef __APPLE__
         + FL_COMMAND
#endif
         + 'v',
     (Fl_Callback *)viewCB, 0},
    {"&Close View", FL_COMMAND + 'w', (Fl_Callback *)closeCB, 0,
     FL_MENU_DIVIDER},
    {"E&xit", FL_COMMAND + 'q', (Fl_Callback *)quitCB, 0},
    {0},

    {"&Edit", 0, 0, 0, FL_SUBMENU},
    {"Cu&t", FL_COMMAND + 'x', (Fl_Callback *)cutCB},
    {"&Copy", FL_COMMAND + 'c', (Fl_Callback *)copyCB},
    {"&Paste", FL_COMMAND + 'v', (Fl_Callback *)pasteCB},
    {"&Delete", 0, (Fl_Callback *)deleteCB},
    {"Preferences", 0, 0, 0, FL_SUBMENU},
    {"Line Numbers", FL_COMMAND + 'l', (Fl_Callback *)lineNumbersCB, 0,
     FL_MENU_TOGGLE},
    {"Word Wrap", 0, (Fl_Callback *)wordWrapCB, 0, FL_MENU_TOGGLE},
    {0},
    {0},

    {"&Search", 0, 0, 0, FL_SUBMENU},
    {"&Find...", FL_COMMAND + 'f', (Fl_Callback *)findCB},
    {"F&ind Again", FL_COMMAND + 'g', find2CB},
    {"&Replace...", FL_COMMAND + 'r', replaceCB},
    {"Re&place Again", FL_COMMAND + 't', replace2CB},
    {0},

    {0}};

Fl_Window *newView() {
  EditorWindow *w = new EditorWindow(660, 400, G_title);

  w->begin();
  Fl_Menu_Bar *m = new Fl_Menu_Bar(0, 0, 660, 30);
  m->copy(MENU_ITEMS, w);
  w->editor = new Fl_Text_Editor(0, 30, 660, 370);
  w->editor->textfont(FL_COURIER);
  w->editor->textsize(TEXT_SIZE);
  // w->editor->wrap_mode(Fl_Text_Editor::WRAP_AT_BOUNDS, 250);
  w->editor->buffer(G_text_buffer);

  w->end();
  w->resizable(w->editor);
  w->size_range(300, 200);
  w->callback((Fl_Callback *)closeCB, w);

  G_text_buffer->add_modify_callback(changedCB, w);
  G_text_buffer->call_modify_callbacks();
  G_num_windows++;
  return w;
}

void loadFileCB(const char *fname) { loadFile(fname, -1); }
