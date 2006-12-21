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

#ifndef __LAUNCHABLE_ITEM_H__
#define __LAUNCHABLE_ITEM_H__

#include <vector>
#include <string>

#include <libosso.h>

#include "launcher-item.h"

// TODO: or better use inheritance?
class LaunchableItem {
public:
  LaunchableItem(LauncherItem *, bool);
 ~LaunchableItem();

  GdkPixbuf *getIcon(int icon_size) const { return myItem->getIcon(icon_size); }

  const std::string& getName() const { return myItem->getName(); }
  const std::string& getComment() const { return myItem->getComment(); }
  const std::string& getService() const { return myItem->getService(); }

  bool isEnabled(void) const { return myEnabled; }

  void enable() { myEnabled = true; }
  void disable() { myEnabled = false; }
  void toggle() { myEnabled = !myEnabled; }

  bool activate(osso_context_t *);

private:
  LauncherItem *myItem;
  bool myEnabled;
};

typedef std::vector<std::pair<std::string, LaunchableItem *> > LaunchableItems;

#endif

// vim:ts=2:sw=2:et
