
#include <string>
#include <vector>

#include <gtk/gtk.h>

#include <hildon-home-plugin/hildon-home-plugin-interface.h>
#include <libosso.h>

#include "launcher-item.h"

extern "C" {
  void *hildon_home_applet_lib_initialize (void *state_data, int *state_size, GtkWidget **widget);
  void hildon_home_applet_lib_deinitialize (void *applet_data);
  void hildon_home_applet_lib_background (void *applet_data);
  void hildon_home_applet_lib_foreground(void *applet_data);
  int hildon_home_applet_lib_save_state(void *applet_data, void **state_data, int *state_size);
  GtkWidget *hildon_home_applet_lib_settings(void *applet_data, GtkWindow *parent);
};

#define SLA_APPLET_DBUS_NAME  "simple-launcher"
#define SLA_APPLET_VERSION    "0.0"

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
  bool initWidget();

  bool startApplication(const std::string& application);

private:
  osso_context_t *myContext;
  GtkWidget *myWidget;

  std::vector<LauncherItem *> myItems;

  static char *ourFiles[];
};

// Hildon home applet interface functions

void *hildon_home_applet_lib_initialize(void *state_data, int *state_size, GtkWidget **widget) {
  SimpleLauncherApplet *applet = new SimpleLauncherApplet();

  if (applet != 0) {
    if (applet->doInit(state_data, state_size)) {
      *widget = applet->getWidget();
    } else {
      delete applet;
      applet = 0;
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

char *SimpleLauncherApplet::ourFiles[] = {
  0
};

SimpleLauncherApplet::SimpleLauncherApplet(): myContext(0), myWidget(0) {
}

bool SimpleLauncherApplet::doInit(void *state_data, int *state_size) {
  if ((myContext = osso_initialize(SLA_APPLET_DBUS_NAME, SLA_APPLET_VERSION, FALSE, NULL)) == 0) {
    g_debug("sla-applet: failed to initialize the osso layer");
    return false;
  }

  if (!initWidget()) {
    return false;
  }

  for (int i = 0 ; ourFiles[i] != 0 ; ++i) {
    LauncherItem *item = new LauncherItem();

    if (item->load(ourFiles[i])) {
      myItems.push_back(item);
    } else {
      delete item;
    }
  }

  // g_signal_connect (applet->myWidget, "do_search", G_CALLBACK (mis_applet_do_search), (gpointer)applet);

  gtk_widget_show_all(myWidget);

  return true;
}

SimpleLauncherApplet::~SimpleLauncherApplet() {
  for (std::vector<LauncherItem *>::iterator it = myItems.begin(); it != myItems.end(); ++it) {
    if (*it != 0) {
      delete *it;
      *it = 0;
    }
  }

  myItems.resize(0);

  if (myWidget != 0) {
    gtk_widget_destroy(myWidget);
    myWidget = 0;
  }

  if (myContext != 0) {
    osso_deinitialize(myContext);
    myContext = 0;
  }
}

bool SimpleLauncherApplet::initWidget() {
  return false;
}

int SimpleLauncherApplet::saveState(void **state_data, int *state_size) {
  if (state_data != 0) {
    *state_data = 0;
  }

  if (state_size != 0) {
    *state_size = 0;
  }

  return 1;
}

GtkWidget *SimpleLauncherApplet::settings(GtkWindow *parent) {
  // TODO: in case we want SimpleLauncherApplet to be configurable, this method
  // should return a gtk_menu_item that would be included in home settings
  // menu.  Method should make sure that when we activate that item, a
  // corresponding dialog appears.
  return 0;
}

bool SimpleLauncherApplet::startApplication(const std::string& application) {
  return osso_application_top(myContext, application.c_str(), 0) == OSSO_OK;
}
