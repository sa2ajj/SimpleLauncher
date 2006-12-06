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

#include <string>

#include <gdk-pixbuf/gdk-pixbuf.h>

#include <libosso.h>

class LauncherItem {
public:
  LauncherItem();
 ~LauncherItem();

  bool activate(osso_context_t *);

  bool load(const std::string&);

  GdkPixbuf *getIcon(int icon_size) const;

  const std::string& getName() const { return myName; }
  const std::string& getComment() const { return myComment; }
  const std::string& getService() const { return myService; }
private:
  std::string myName, myComment, myIcon, myService;
};

#endif

// vim:ts=2:sw=2:et
