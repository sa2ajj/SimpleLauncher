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

#ifndef __APPLICATIONITEM_H__
#define __APPLICATIONITEM_H__

#include "BasicItem.h"

class ApplicationItemFactory : public BasicItemFactory {
public:
 ~ApplicationItemFactory();

  const std::string& factoryName() const;
protected:
	BasicItem *createItem(const std::string&) const;
};

class ApplicationItem : public BasicItem {
  friend class ApplicationItemFactory;

protected:
  ApplicationItem(const std::string& itemID);
 ~ApplicationItem();

  bool load();

  std::string getName() const;
  std::string getComment() const;
  GdkPixbuf *getIcon(int iconSize) const;

  bool isSane() const { return !myID.empty() && (!myService.empty() || !myExec.empty()); }

  void activate(osso_context_t *);
private:
  std::string myService, myExec;
};

#endif
