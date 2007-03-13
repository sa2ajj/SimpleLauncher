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
#include <map>
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

typedef struct {
  typedef std::vector<std::string> Names;
  typedef std::map<std::string, LauncherItem *> Items;

  Names myNames;
  Items myItems;

  size_t size() { return myNames.size(); }

  LauncherItem *operator[](int index) {
    return myItems[myNames[index]];
  }

  std::string& name(int index) {
    return myNames[index];
  }

  void add(std::string& name, LauncherItem *item) {
    myNames.push_back(name);
    myItems[name] = item;
  }

  void swap(size_t i1, size_t i2) {
    std::swap(myNames[i1], myNames[i2]);
  }

  void clear() {
    for (Items::iterator it = myItems.begin(); it != myItems.end(); ++it) {
      if (it->second != NULL) {
        delete it->second;
        it->second = NULL;
      }
    }

    myNames.resize(0);
    myItems.clear();
  }
} LauncherItems;

#endif

// vim:ts=2:sw=2:et
