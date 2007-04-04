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

#ifndef _GCONF_WRAPPER_H_
#define _GCONF_WRAPPER_H_

#include <string>

#include <gconf/gconf-client.h>

class GConfClientWrapper {
  friend class GConfKey;

public:
  GConfClientWrapper();
 ~GConfClientWrapper();

  GConfKey getKey(const std::string&);

protected:
  bool getBool(const std::string& name);
  void setBool(const std::string& name, bool);

  int getInt(const std::string& name);
  void setInt(const std::string& name, int);

private:
  GConfClient *myClient;
};

class GConfKey {
public:
  GConfKey(GConfClientWrapper&, const std::string&);
  GConfKey(const GConfKey& what) : myWrapper(what.myWrapper), myPath(what.myPath) { }
 ~GConfKey();

  GConfKey& operator = (const GConfKey& what) {
    myWrapper = what.myWrapper;
    myPath = what.myPath;

    return *this;
  }

  bool getBool(const std::string& name, bool defvalue = false);
  void setBool(const std::string& name, bool value);

  int getInt(const std::string& name, int defvalue = 0);
  void setInt(const std::string& name, int value);

private:
  GConfClientWrapper& myWrapper;
  std::string myPath;
};

#endif

// vim:ts=2:sw=2:et
