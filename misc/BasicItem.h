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

#ifndef __BASICITEM_H__
#define __BASICITEM_H__

#include <string>

#include <gdk-pixbuf/gdk-pixbuf.h>

class BasicItem {
protected:
  BasicItem(const std::string& type, const std::string& id) : myType(type), myID(id), myEnabled(false) {}
public:
  virtual ~BasicItem() {}

	const std::string& getType() const { return myType; }
	const std::string& getID() const { return myID; }

  virtual std::string getName() const = 0;
  virtual std::string getComment() const = 0;
  virtual GdkPixbuf *getIcon(int iconSize) const = 0;

  virtual void activate() = 0;

  virtual bool isSane() const = 0;

  bool isEnabled(void) const { return myEnabled; }

  virtual void enable() { myEnabled = isSane(); }
  virtual void disable() { myEnabled = false; }
  void toggle() {
    if (myEnabled) {
      disable();
    } else {
      enable();
    }
  }
private:
  BasicItem();  // We do not want people to create these objects :)

	const std::string& myType;
	const std::string& myID;
  bool myEnabled;
};

class BasicItemFactory {
private:
	BasicItemFactory();

public:
	virtual const string::std& factoryName() const = 0;

	virtual BasicItem *createItem(const std::string&) const = 0;

protected:
	static void registerFactory(const std::string&, BasicItemFactory *);

protected:
	std::map<std::string, BasicItemFactory *> ourFactories;
};

typedef struct BasicItemCollection {
  typedef std::vector<std::string> Names;
  typedef std::map<std::string, BasicItem *> Items;

  Names myNames;
  Items myItems;

  bool exists(const std::string& name) {
    return std::find(myNames.begin(), myNames.end(), name) != myNames.end();
  }

  size_t size() { return myNames.size(); }

  BasicItem *operator[](int index) {
    return myItems[myNames[index]];
  }

  std::string& name(int index) {
    return myNames[index];
  }

  void add(const std::string& name, BasicItem *item) {
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

  BasicItemCollection& operator=(const BasicItemCollection& that) {
    myNames = that.myNames;
    myItems = that.myItems;

    return *this;
  }
};

#endif
