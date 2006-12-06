// This file is a part of Simple Launcher
//
// Copyright (C) 2006, Mikhail Sobolev
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

// vim:ts=2:sw=2:et
