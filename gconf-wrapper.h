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

class GConfWrapper {
public:
  GConfWrapper(const std::string&);
 ~GConfWrapper();

  bool getBool(const std::string& name);
  void setBool(const std::string& name, bool);

  int getInt(const std::string& name);
  void setInt(const std::string& name, int);
};

#endif

// vim:ts=2:sw=2:et
