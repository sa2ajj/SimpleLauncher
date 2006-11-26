#include <iostream>

#include "launcher-item.h"

char *files[] = {
  "/usr/share/applications/hildon/hildon-control-panel.desktop",
  "/usr/share/applications/hildon/hwr-teacher.desktop",
  "/usr/share/applications/hildon/osso-backup.desktop",
  "/usr/share/applications/hildon/osso-global-search.desktop",
  0
};

int main() {
  for (int i = 0 ; files[i] != 0 ; ++i) {
    LauncherItem *item = new LauncherItem();

    if (item->load(files[i])) {
      std::cout << "Loaded " << files[i] << std::endl
                << " name: " << item->getName() << std::endl
                << " comment: " << item->getComment() << std::endl
                << " service: " << item->getService() << std::endl;
    } else {
      std::cout << "Failed to load " << files[i] << std::endl;
    }

    delete item;
  }

  return 0;
}
