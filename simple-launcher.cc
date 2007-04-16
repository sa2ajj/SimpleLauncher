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
#include "launchable-item.h"
#include "settings-dialog.h"
#include "gconf-wrapper.h"

#define SL_APPLET_DBUS_NAME  "simple-launcher"
#define SL_APPLET_VERSION    "0.0"

#define SL_APPLET_GCONF_PATH  "/apps/simple-launcher"

class SimpleLauncherApplet {
public:
  SimpleLauncherApplet(const GConfKey&);
 ~SimpleLauncherApplet();

  bool doInit(void *state_data, int *state_size);

  void background() {}
  void foreground() {}
  int saveState(void **state_data, int *state_size);
  GtkWidget *settings(GtkWindow *parent);

  GtkWidget *getWidget() { return myWidget; }

private:
  static void addItem(LauncherItems&, const std::string&, bool);

  void loadConfig();
  void saveConfig();

  static void updateItems(LauncherItems&);
  static void processDirectory(LauncherItems&, const std::string&);

  bool initWidget();
  void updateWidget();

  void buttonPressed(GtkWidget *button, GdkEventButton *event);
  void runDialog();

  static void _button_pressed(GtkWidget *button, GdkEventButton *event, void *self);
  static void _run_dialog(GtkMenuItem *, void *);

private:
  // GConfClientWrapper myClient;
  // GConfKey myMainSettings;

  osso_context_t *myContext;

  GtkWidget *myWidget;
  GtkWindow *myParent;

  LauncherItems myItems;

  GConfBooleanOption myTransparent;
  // bool myShowInfobanner; // FIXME: to implement
  GConfIntegerOption myIconSize;

  static char *ourDirs[];
};

// Hildon home applet interface functions

void *hildon_home_applet_lib_initialize(void *state_data, int *state_size, GtkWidget **widget) {
  GConfKey baseKey(SL_APPLET_GCONF_PATH);

  SimpleLauncherApplet *applet = new SimpleLauncherApplet(baseKey);

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

// SimpleLauncherApplet::SimpleLauncherApplet() : myMainSettings(myClient.getKey(SL_APPLET_GCONF_PATH)), myContext(NULL), myWidget(NULL), myParent(NULL) {
SimpleLauncherApplet::SimpleLauncherApplet(const GConfKey& base) : myContext(NULL), myWidget(NULL), myParent(NULL), myTransparent(base, "transparent", true), myIconSize(base, "icon_size", 48) {
}

bool SimpleLauncherApplet::doInit(void *state_data, int *state_size) {
  if ((myContext = osso_initialize(SL_APPLET_DBUS_NAME, SL_APPLET_VERSION, FALSE, NULL)) == NULL) {
    g_debug("sla-applet: failed to initialize the osso layer");
    return false;
  }

  loadConfig();

  if (!initWidget()) {
    return false;
  }

  return true;
}

SimpleLauncherApplet::~SimpleLauncherApplet() {
  myItems.clear();
#if 0
  // This does not seem to be necessary
  if (myWidget != NULL) {
    gtk_widget_destroy(myWidget);
    myWidget = NULL;
  }
#endif
  if (myContext != NULL) {
    osso_deinitialize(myContext);
    myContext = NULL;
  }
}

void SimpleLauncherApplet::addItem(LauncherItems& items, const std::string& name, bool enabled) {
  if (!items.exists(name)) {
    LaunchableItem *item = new LaunchableItem();

    item->load(name);

    if (enabled) {
      item->enable();
    } else {
      item->disable();
    }

    items.add(name, item);
  }
}

// FIXME: this probably should be done somehow differently
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

      addItem(myItems, buffer, (p != NULL && (*p == '1' || *p == 'y' || *p == 'Y')));
    }

    delete [] buffer;
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

void SimpleLauncherApplet::updateItems(LauncherItems& items) {
  for (int i = 0 ; ourDirs[i] != NULL ; ++i) {
    processDirectory(items, ourDirs[i]);
  }
}

void SimpleLauncherApplet::processDirectory(LauncherItems& items, const std::string& dirname) {
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
        addItem(items, namePrefix+shortName, false);
      }
    }

    closedir(dir);
  }
}

bool SimpleLauncherApplet::initWidget() {
  myWidget = gtk_hbox_new(false, 0);

  if (myWidget != NULL) {
    updateWidget();
  }

  return myWidget != NULL;
}

void SimpleLauncherApplet::updateWidget() {
  gtk_container_foreach(GTK_CONTAINER(myWidget), (GtkCallback)gtk_widget_destroy, NULL);

  GtkSizeGroup *group = gtk_size_group_new(GTK_SIZE_GROUP_BOTH);

  int button_no = 0;

  for (size_t i = 0 ; i < myItems.size() ; ++i) {
    LauncherItem *item = myItems[i];

    if (item != NULL && item->isEnabled()) {
      GtkWidget *button = gtk_event_box_new();

      gtk_widget_set_events(button, GDK_BUTTON_PRESS_MASK);
      g_signal_connect(button, "button-press-event", G_CALLBACK(_button_pressed), this);

      gtk_event_box_set_visible_window(GTK_EVENT_BOX(button), !myTransparent.value());

      {
        GdkPixbuf *pixbuf = item->getIcon(myIconSize.value());
        gtk_container_add(GTK_CONTAINER(button), gtk_image_new_from_pixbuf(pixbuf));
        g_object_unref(G_OBJECT(pixbuf));
      }

      gtk_object_set_user_data(GTK_OBJECT(button), item);

      gtk_size_group_add_widget(group, button);

      gtk_box_pack_start(GTK_BOX(myWidget), GTK_WIDGET(button), false, false, 0);

      ++button_no;
    }
  }

  g_object_unref(G_OBJECT(group));

  int totalSize = myIconSize.value();

  if (button_no == 0) {
    gtk_widget_set_size_request(myWidget, totalSize, totalSize);
  } else {
    gtk_widget_set_size_request(myWidget, button_no*totalSize, totalSize);
  }

  gtk_widget_show_all(myWidget);
}

void SimpleLauncherApplet::_button_pressed(GtkWidget *button, GdkEventButton *event, void *self) {
  ((SimpleLauncherApplet *)self)->buttonPressed(button, event);
}

void SimpleLauncherApplet::buttonPressed(GtkWidget *button, GdkEventButton *event) {
  if (button != NULL && event->button == 1) {
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
  myParent = parent;  // FIXME: Ugly piece of code :(

  GtkWidget *menuItem = gtk_menu_item_new_with_label("Launcher settings...");

  g_signal_connect(menuItem, "activate", G_CALLBACK(_run_dialog), this);

  return menuItem;
}

void SimpleLauncherApplet::_run_dialog(GtkMenuItem *, void *self) {
  ((SimpleLauncherApplet *)self)->runDialog();
}

void SimpleLauncherApplet::runDialog() {
  // We update the items before using them to avoid a small memory leak
  // FIXME: deal with the situation in a better way (figure it out first :))
  updateItems(myItems);       // User requested 'settings', let's give her the latest stuff :)

  LauncherItems newItems = myItems;

  // TODO: make it nicer... this code is ugly :(
  SettingsDialog dialog(myParent, newItems, myTransparent, myIconSize);

  switch (dialog.run()) {
    case GTK_RESPONSE_OK:
      myItems = newItems;
      dialog.updateValues();  // FIXME: hackish :( make it better

      saveConfig();   // save it immediately!
      updateWidget();
      break;

    case GTK_RESPONSE_CANCEL:
      break;

    default:
      ;     // FIXME: do I want to do anything in here?
  }

  newItems.clear();
}

// vim:ts=2:sw=2:et
