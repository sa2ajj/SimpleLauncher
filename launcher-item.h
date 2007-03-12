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

#ifndef __LAUNCHER_ITEM_H__
#define __LAUNCHER_ITEM_H__

#include <vector>
#include <string>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkicontheme.h>

class LauncherItem {
public:
  LauncherItem();
  virtual ~LauncherItem();

  bool load(const std::string&);

  GdkPixbuf *getIcon(int icon_size) const;

  const std::string& getFileName() const { return myFileName; }
  const std::string& getName() const { return myName; }
  const std::string& getComment() const { return myComment; }
  const std::string& getService() const { return myService; }

  bool isEnabled(void) const { return myEnabled; }

  void enable() { myEnabled = checkSanity(); }
  void disable() { myEnabled = false; }
  void toggle() {
    if (myEnabled) {
      disable();
    } else {
      enable();
    }
  }

private:
  bool checkSanity(void) { return !(myName.empty() || myIcon.empty() || myService.empty()); }

private:
  std::string myFileName, myName, myComment, myIcon, myService;
  bool myEnabled;

  static GtkIconTheme *ourTheme;
};

typedef std::vector<std::pair<std::string, LauncherItem *> > LauncherItems;

#endif

// vim:ts=2:sw=2:et
