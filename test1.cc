#include <iostream>
#include <map>

#include <string.h>
#include <dirent.h>

#include <gtk/gtkmain.h>
#include <gtk/gtkdialog.h>
#include <gtk/gtkstock.h>

#include "sla-list.h"
#include "launcher-item.h"

static std::string appdir = "/usr/share/applications";

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  DIR *handle = opendir(appdir.c_str());

  if (handle != 0) {
    LauncherItems apps;

    struct dirent *entry;

    while ((entry = readdir(handle)) != 0) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }

      LauncherItem *item = new LauncherItem();

      if (item->load(appdir + "/" + entry->d_name)) {
        std::cout << "Loaded " << entry->d_name << std::endl;
      } else {
        std::cout << "Failed to load " << entry->d_name << std::endl;
      }

      apps.push_back(std::pair<std::string, LauncherItem *>(entry->d_name, item));
    }

    if (!apps.empty()) {
      SLAList sla_list(26, apps);

      GtkDialog *dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Launcher settings", 0, (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 0));

      gtk_container_add(GTK_CONTAINER(dialog->vbox), sla_list.getWidget());

      if (gtk_dialog_run(dialog) == GTK_RESPONSE_ACCEPT) {
        for (LauncherItems::const_iterator it = apps.begin(); it != apps.end(); ++it) {
          std::cout << it->first << (it->second->isEnabled() ? " active" : " passive") << std::endl;
        }
      }

      gtk_widget_destroy(GTK_WIDGET(dialog));
    }
  } else {
    std::cerr << "Cannot list applications directory" << std::endl;
  }

  return 0;
}
