// This file is a part of Simple Launcher
//
// Copyright (C) 2006, 2007, Mikhail Sobolev
//
// Simple Launcher is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <string>
#include <vector>
#include <fstream>

#include <dirent.h>

#include <gtk/gtk.h>

#include <hildon-home-plugin/hildon-home-plugin-interface.h>
#include <libosso.h>

#include "launcher-item.h"
#include "sla-list.h"
#include "launchable-item.h"

#define SL_APPLET_DBUS_NAME  "simple-launcher"
#define SL_APPLET_VERSION    "0.0"
#define SL_APPLET_ICON_SIZE  26
#define SL_APPLET_BORDER_SIZE  14
#define SL_APPLET_CANVAS_SIZE  (SL_APPLET_BORDER_SIZE+SL_APPLET_BORDER_SIZE)

class SimpleLauncherApplet {
public:
  SimpleLauncherApplet();
 ~SimpleLauncherApplet();

  bool doInit(void *state_data, int *state_size);

  void background() {}
  void foreground() {}
  int saveState(void **state_data, int *state_size);
  GtkWidget *settings(GtkWindow *parent);

  GtkWidget *getWidget() { return myWidget; }

private:
  void addItem(const std::string&, bool);

  void loadConfig();
  void saveConfig();

  void processDirectory(const std::string&);

  bool initWidget();
  void updateWidget();

  void buttonClicked(GtkToolButton *);
  void runDialog();

  static void _button_clicked(GtkToolButton *, void *);
  static void _run_dialog(GtkMenuItem *, void *);

private:
  osso_context_t *myContext;
  GtkWidget *myWidget;
  GtkWindow *myParent;

  LauncherItems myItems;

  static char *ourDirs[];
};

// Hildon home applet interface functions

void *hildon_home_applet_lib_initialize(void *state_data, int *state_size, GtkWidget **widget) {
  SimpleLauncherApplet *applet = new SimpleLauncherApplet();

  if (applet != NULL) {
    if (applet->doInit(state_data, state_size)) {
      *widget = applet->getWidget();
    } else {
      delete applet;
      applet = NULL;
    }
  }

  return (void*)applet;
}

void hildon_home_applet_lib_deinitialize(void *applet_data) {
  SimpleLauncherApplet *applet = (SimpleLauncherApplet *)applet_data;

  delete applet;
}

void hildon_home_applet_lib_background(void *applet_data) {
  ((SimpleLauncherApplet *)applet_data)->background();
}

void hildon_home_applet_lib_foreground (void *applet_data) {
  ((SimpleLauncherApplet *)applet_data)->foreground();
}

GtkWidget *hildon_home_applet_lib_settings(void *applet_data, GtkWindow *parent) {
  return ((SimpleLauncherApplet *)applet_data)->settings(parent);
}

int hildon_home_applet_lib_save_state (void *applet_data, void **state_data, int *state_size) {
  return ((SimpleLauncherApplet *)applet_data)->saveState(state_data, state_size);
}

// SimpleLauncherApplet implementation

char *SimpleLauncherApplet::ourDirs[] = {
  "/usr/share/applications/hildon",
  NULL
};

SimpleLauncherApplet::SimpleLauncherApplet(): myContext(NULL), myWidget(NULL), myParent(NULL) {
}

bool SimpleLauncherApplet::doInit(void *state_data, int *state_size) {
  if ((myContext = osso_initialize(SL_APPLET_DBUS_NAME, SL_APPLET_VERSION, FALSE, NULL)) == NULL) {
    g_debug("sla-applet: failed to initialize the osso layer");
    return false;
  }

  loadConfig();

  for (int i = 0 ; ourDirs[i] != NULL ; ++i) {
    processDirectory(ourDirs[i]);
  }

  if (!initWidget()) {
    return false;
  }

  gtk_widget_show_all(myWidget);

  return true;
}

SimpleLauncherApplet::~SimpleLauncherApplet() {
  myItems.clear();

  if (myWidget != NULL) {
    gtk_widget_destroy(myWidget);
    myWidget = NULL;
  }

  if (myContext != NULL) {
    osso_deinitialize(myContext);
    myContext = NULL;
  }
}

void SimpleLauncherApplet::addItem(const std::string& name, bool enabled) {
  if (!myItems.exists(name)) {
    LaunchableItem *item = new LaunchableItem();

    item->load(name);

    if (enabled) {
      item->enable();
    } else {
      item->disable();
    }

    myItems.add(name, item);
  }
}

static char *configFileName="/home/user/.slarc";

void SimpleLauncherApplet::loadConfig() {
  std::ifstream config(configFileName);

  if (config) {
    char *buffer = new char [1024];

    while (config.getline(buffer, 1024)) {
      char *p = strchr(buffer, ',');

      if (p != NULL) {
        *p++ = '\0';
      }

      addItem(buffer, (p != NULL && (*p == '1' || *p == 'y' || *p == 'Y')));

    }

    delete buffer;
  }
}

void SimpleLauncherApplet::saveConfig() {
  // TODO: make saving config an atomic operation
  std::ofstream config(configFileName);

  if (config) {
    for (size_t i = 0 ; i < myItems.size() ; ++i) {
      config << myItems.name(i) << ',' << myItems[i]->isEnabled() << std::endl;
    }
  }
}

void SimpleLauncherApplet::processDirectory(const std::string& dirname) {
  DIR *dir = opendir(dirname.c_str());

  if (dir != NULL) {
    const std::string namePrefix = dirname + "/";
    std::string shortName;
    std::string desktopExtension = ".desktop";
    const dirent *file;

    while ((file = readdir(dir)) != 0) {
      shortName = file->d_name;
      if ((shortName == ".") || (shortName == "..")) {
        continue;
      }

      if ((shortName.length() >= desktopExtension.length()) && (shortName.compare(shortName.length() - desktopExtension.length(), desktopExtension.length(), desktopExtension) == 0)) {
        addItem(namePrefix+shortName, false);
      }
    }

    closedir(dir);
  }
}

bool SimpleLauncherApplet::initWidget() {
  myWidget = gtk_frame_new(NULL);

  if (myWidget != NULL) {
    gtk_frame_set_shadow_type(GTK_FRAME(myWidget), GTK_SHADOW_ETCHED_IN);

    updateWidget();
  }

  return myWidget != NULL;
}

void SimpleLauncherApplet::updateWidget() {
  GtkWidget *child = gtk_bin_get_child(GTK_BIN(myWidget));

  if (child != NULL) {
    gtk_container_remove(GTK_CONTAINER(myWidget), child);
    gtk_widget_destroy(child);
  }

  int button_no = 0;
  GtkToolbar *toolbar = GTK_TOOLBAR(gtk_toolbar_new());

  for (size_t i = 0 ; i < myItems.size() ; ++i) {
    LauncherItem *item = myItems[i];

    if (item != NULL && item->isEnabled()) {
      GtkToolItem *button = gtk_tool_button_new(gtk_image_new_from_pixbuf(item->getIcon(SL_APPLET_ICON_SIZE)), NULL);

      gtk_object_set_user_data(GTK_OBJECT(button), item);
      g_signal_connect(button, "clicked", G_CALLBACK(_button_clicked), this);

      gtk_toolbar_insert(toolbar, button, -1);

      ++button_no;
    }
  }

  if (button_no) {
    gtk_container_add(GTK_CONTAINER(myWidget), GTK_WIDGET(toolbar));
    gtk_widget_set_size_request(myWidget, button_no*(SL_APPLET_ICON_SIZE+SL_APPLET_CANVAS_SIZE), SL_APPLET_ICON_SIZE+SL_APPLET_CANVAS_SIZE);
  } else {
    gtk_widget_destroy(GTK_WIDGET(toolbar));
  }
}

void SimpleLauncherApplet::_button_clicked(GtkToolButton *button, void *self) {
  ((SimpleLauncherApplet *)self)->buttonClicked(button);
}

void SimpleLauncherApplet::buttonClicked(GtkToolButton *button) {
  if (button != NULL) {
    LaunchableItem *item = (LaunchableItem *)gtk_object_get_user_data(GTK_OBJECT(button));

    if (item != NULL) {
      item->activate(myContext);
    }
  }
}

int SimpleLauncherApplet::saveState(void **state_data, int *state_size) {
  if (state_data != NULL) {
    *state_data = NULL;
  }

  if (state_size != NULL) {
    *state_size = 0;
  }

  return 1;
}

GtkWidget *SimpleLauncherApplet::settings(GtkWindow *parent) {
  // TODO: in case we want SimpleLauncherApplet to be configurable, this method
  // should return a gtk_menu_item that would be included in home settings
  // menu.  Method should make sure that when we activate that item, a
  // corresponding dialog appears.
  myParent = parent;  // FIXME: Ugly piece of code :(

  GtkWidget *menuItem = gtk_menu_item_new_with_label("Launcher settings...");

  g_signal_connect(menuItem, "activate", G_CALLBACK(_run_dialog), this);

  return menuItem;
}

void SimpleLauncherApplet::_run_dialog(GtkMenuItem *, void *self) {
  ((SimpleLauncherApplet *)self)->runDialog();
}

void SimpleLauncherApplet::runDialog() {
  SLAList list(SL_APPLET_ICON_SIZE, myItems);

  GtkDialog *dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Launcher Settings", myParent, (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL));

  gtk_container_add(GTK_CONTAINER(dialog->vbox), list.getWidget());

  gtk_widget_set_size_request(GTK_WIDGET(dialog), 540, 257);

  int response = gtk_dialog_run(dialog);

  gtk_widget_destroy(GTK_WIDGET(dialog));

  switch (response) {
    case GTK_RESPONSE_OK:
      break;

    case GTK_RESPONSE_CANCEL:
      break;

    default:
      ;     // FIXME: do I want to do anything in here?
  }
}

// vim:ts=2:sw=2:et
